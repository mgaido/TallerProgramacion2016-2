#include "Cliente.h"

Cliente::Cliente(std::string cHost, int cPuerto) {
	conectado = false;
	puerto = cPuerto;
	host = cHost;

	vista = nullptr;
}

Cliente::~Cliente() {
}

void Cliente::iniciar() {
	conectar();
	if (conectado) {
		vista = new Vista(eventosTeclado);
		t_enviarEventos = std::thread(&Cliente::enviarEventos, this);
		t_recibirAct = std::thread(&Cliente::recibirActualizaciones, this);
		vista->iniciar();
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
			break;
		}
	}
}

void Cliente::recibirActualizaciones() {
	while (conectado) {
		try {
			Bytes bytes = con.recibir();

			int comando;
			bytes.get(comando);
			if (comando == UPD) {
				std::vector<Actualizacion> actualizaciones;
				bytes.getAll(actualizaciones);
				vista->recibirActualizaciones(actualizaciones);
			}
		} catch (SocketException&) {}
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
	if (conectado) {
		conectado = false;

		debug("Esperando que termine thread enviarEventos");
		eventosTeclado.cerrar();
		t_enviarEventos.join();
		debug("Thread enviarEventos termino");
		
		debug("Esperando que termine thread recibirActualizaciones");
		con.cancelarRecepcion();
		t_recibirAct.join();
		debug("Thread recibirActualizaciones termino");

		vista->detener();
		delete vista;

		Bytes bytes;
		bytes.put(BYE);
		try {
			con.enviar(bytes);
			info("Desconexion exitosa con: " + host, true);
		} catch (SocketException&){}
		con.cerrar();
	} else {
		warn("No hay una Conexion abierta");
	}
}


