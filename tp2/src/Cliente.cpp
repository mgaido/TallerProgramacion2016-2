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
}

void Cliente::iniciar() {
	conectar();
	if (conectado) {
		HandshakeRequest handshakeRequest;
		usuario.copy(handshakeRequest.nombre, sizeof handshakeRequest.nombre);
		handshakeRequest.nombre[usuario.size()] = '\0';

		try {
			Bytes bytes;
			bytes.put(handshakeRequest);
			debug("Enviando handshake");
			con.enviar(bytes);

			debug("Recibiendo respuesta a handshake");
			bytes = con.recibir();
			HandshakeResponse handshakeResponse;
			bytes.getSerializable(handshakeResponse);

			if (handshakeResponse.aceptado) {
				debug("Usuario aceptado");
				Bytes bytes;
				bytes.put(INIT);
				con.enviar(bytes);
				vista = new Vista(eventosTeclado);
				t_enviarEventos = std::thread(&Cliente::enviarEventos, this);
				t_recibirEstado = std::thread(&Cliente::recibirEstado, this);
				vista->iniciar(); //Bloquea
			} else {
				conectado = false;
				error("El servidor no acepto un nuevo usuario", true);
			}
		} catch (SocketException&) {
			conectado = false;
			error("Error de conexion durante el handshake");
		}
	}
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
		try {
			Bytes bytes = con.recibir();
			int comando;
			bytes.get(comando);
			if (comando == UPD) {
				std::vector<EstadoObj> estado;
				bytes.getAll(estado);
				vista->recibirEstado(estado);
			} else if (comando == INIT) {
				bytes.getSerializable(config);
				/*
				std::cout << "IdMapa: " << config.getConfigCapas().at(0).idCapa << std::endl;
				std::cout << "zIndexMapa: " << config.getConfigCapas().at(0).zIndexCapa << std::endl;
				std::cout << "IdSprite: " << config.getConfigSprites().at(0).idSprite << std::endl;
				std::cout << "zSprite: " << config.getConfigSprites().at(0).zIndexSprite << std::endl;
				std::cout << "anchoSprite: " << config.getConfigSprites().at(0).anchoSprite << std::endl;
				std::cout << "altoSprite: " << config.getConfigSprites().at(0).altoSprite << std::endl;
				*/
			}
		} catch (SocketException&) {
			conectado = false;
			vista->detener();
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

		if (vista != nullptr) {
			vista->detener();
			delete vista;
		}

		con.cerrar();
	}
}


