#include "Cliente.h"

Cliente::Cliente(std::string cHost, int cPuerto, std::string usuario) {
	cerrado = false;
	conectado = false;
	puerto = cPuerto;
	host = cHost;
	this->usuario = usuario;
	vista = nullptr;
}

Cliente::~Cliente() {
	if (vista != nullptr)
		delete vista;
}

void Cliente::iniciar() {
	conectar();
	t_enviarEventos = std::thread(&Cliente::enviarEventos, this);
	t_recibirEstado = std::thread(&Cliente::recibirEstado, this);

	if (conectado) do {
		reconectar = false;

		HandshakeRequest handshakeRequest;
		handshakeRequest.setNombre(usuario);
		try {
			Bytes bytes;
			bytes.put(HSK_REQ);
			bytes.put(handshakeRequest);
			con.enviar(bytes);
			debug("Handshake enviado");

			int comando = -1;
			do {
				bytes = con.recibir();
				bytes.get(comando);
			} while (comando != HSK_RES);

			HandshakeResponse handshakeResponse;
			bytes.getSerializable(handshakeResponse);
			debug("Respuesta a handshake recibida");

			if (handshakeResponse.isAceptado()) {
				debug("Usuario aceptado");
				vista = new Vista(eventosTeclado, usuario, handshakeResponse.getIdJugador(), handshakeResponse.getConfiguracion());
				vista->iniciar(); //Bloquea
				info("Vista cerrada");
				delete vista;
				vista = nullptr;
			} else {
				conectado = false;
				error("El servidor no acepto un nuevo usuario", true);
			}
		} catch (SocketException&) {
			conectado = false;
			error("Error de conexion durante el handshake");
		}
	} while (reconectar);
}

void Cliente::enviarEventos() {
	while (conectado) {
		try {
			int estado = eventosTeclado.desencolar();
			Bytes bytes;
			bytes.put(KEY);
			bytes.put(estado);
			con.enviar(bytes);
		} catch (ColaCerrada&) {
			conectado = false;
		} catch (SocketException&) {
			conectado = false;
		}
	}
}

void Cliente::recibirEstado() {
	while (conectado) {
		if (vista == nullptr || reconectar) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		try {
			Bytes bytes = con.recibir();
			int comando;
			bytes.get(comando);
			if (comando == UPD) {
				EstadoJuego estado;
				bytes.get(estado);
				int offsetVista;
				bytes.get(offsetVista);
				std::vector<EstadoObj> estadoObjs;
				bytes.getAll(estadoObjs);
				std::vector<InfoJugador> hudInfo;
				bytes.getAll(hudInfo);
				vista->nuevoEstado(estado, offsetVista, estadoObjs, hudInfo);
			} else if (comando == RLD) {
				info("Reiniciando vista");
				reconectar = true;
				vista->detener();
			} else if (comando == BYE) {
				info("Cerrando vista");
				conectado = false;
				vista->detener();
			} else {
				warn("Comando invalido: " + std::to_string(comando));
			}
		} catch (SocketException&) {
			if (! reconectar) {
				conectado = false;
				if (vista != nullptr)
					vista->detener();
			}
		}
	}
}

void Cliente::conectar() {
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(puerto);

	std::string funcion = "inet_pton";
	int respuesta = inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr.s_addr)) - 1;

	if (respuesta >= 0) {
		funcion = "socket";
		respuesta = -1;
		SOCKET socketD = socket(AF_INET, SOCK_STREAM, 0);

		if (socketD != INVALID_SOCKET) {
			setTcpNoDelay(socketD);
			setTcpKeepAlive(socketD);

			funcion = "connect";
			respuesta = connect(socketD, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

			if (respuesta == 0) {
				conectado = true;
				con.setSocket(socketD);
				info("Conectado a " + host + ":" + std::to_string(puerto));
			}
		}
	}

	if (respuesta < 0) {
		error("No se pudo establecer la conexion con el Servidor", true);
		error("Error " + std::to_string(getLastError()) + " en " + funcion);
	}
}

void Cliente::desconectar() {
	if (! cerrado) {
		cerrado = true;

		if (conectado) {
			conectado = false;
			Bytes bytes;
			bytes.put(BYE);
			try {
				con.enviar(bytes);
				info("Desconexion exitosa con: " + host);
			} catch (SocketException&){}
		}

		eventosTeclado.cerrar();
		con.cancelarRecepcion();

		if (t_enviarEventos.joinable()) {
			debug("Esperando que termine thread enviarEventos");
			t_enviarEventos.join();
			debug("Thread enviarEventos termino");
		}

		if (t_recibirEstado.joinable()) {
			debug("Esperando que termine thread recibirEstado");
			t_recibirEstado.join();
			debug("Thread recibirEstado termino");
		}

		con.cerrar();
	}
}


