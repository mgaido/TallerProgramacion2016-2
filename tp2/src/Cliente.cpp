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
		eventosTeclado.cerrar();
		t_enviarEventos.join();
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
		} catch (ColaCerrada) {
			break;
		}
	}
}

void Cliente::recibirActualizaciones() {
	while (conectado) {
		try {
			con.recibir();
		} catch (SocketException) {}
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
		//info("Desconexion exitosa con: " + host, true);
		eventosTeclado.cerrar();
		vista->detener();
		delete vista;
		con.cerrar();
		t_recibirAct.join();
	} else {
		warn("No hay una Conexion abierta");
	}
}


