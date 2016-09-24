#include "Cliente.h"

Cliente::Cliente(std::string cHost, int cPuerto) {
	conectado = false;
	detenido = false;
	socketD = INVALID_SOCKET;
	puerto = cPuerto;
	host = cHost;

	pingThread = std::thread(&Cliente::ping, this);
}

Cliente::~Cliente() {
	detenido = true;
	pingThread.join();
}

void Cliente::iniciar() {

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
		socketD = socket(AF_INET, SOCK_STREAM, 0);

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
		enviarComando(BYE);
		if (conectado) {
			conectado = false;
			info("Desconexion exitosa con: " + host, true);
		}
		closesocket(socketD);
	} else {
		warn("No hay una Conexion abierta");
	}
}

void Cliente::ping() {
	int i = 1;
	while (! detenido) {
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		if (conectado && (i % 4) == 0) {
			i = 0;
			std::vector<std::string> resp = enviarComando(PING);
			if (resp[0] != SUCCESS)
				error(resp[1], true);
		}
		i++;
	}
}

std::vector<std::string> Cliente::enviarComando(std::string msg) {
	std::vector<std::string> resp;
	try {
		conLock.lock();
		con.enviar(msg);
		msg = con.recibir();
		conLock.unlock();
		resp = split(msg, DELIM);
	}  catch (SocketException e) {
		conLock.unlock();
		conectado = false;
		resp.push_back(FAIL);
		resp.push_back("Conexion perdida con el servidor");
	}

	return resp;
}
