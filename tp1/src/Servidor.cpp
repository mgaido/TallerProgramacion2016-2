#include "Servidor.h"

Servidor::Servidor(int puerto) {
	this->puerto = puerto;
	detenido = false;
	socketD = INVALID_SOCKET;
}

Servidor::~Servidor() {
	detener();
}

void Servidor::iniciar() {
	int response = crearSocket();
	if (response < 0) {
		std::cerr << "No se pudo iniciar la conexión" << std::endl;
	} else {
		thread = std::thread(&Servidor::aceptarConexiones, this);
		std::cout << "Presiona enter para terminar..." << std::endl;
		std::string msg;
		std::getline(std::cin, msg);
	}
	detener();
}

int Servidor::crearSocket() {
	int response = -1;

	std::string funcion = "socket";
	socketD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketD != INVALID_SOCKET) {
		response = 0;

		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(puerto);

		funcion = "bind";
		response = bind(socketD, (struct sockaddr*) &serverAddress,
				sizeof(serverAddress));
		if (response == 0) {
			funcion = "listen";
			response = listen(socketD, 4);
		}
	}

	if (response < 0) {
		socketD = INVALID_SOCKET;
		error("Error llamando a " + funcion + " con código " + std::to_string(getLastError()));
	}

	return response;
}

void Servidor::aceptarConexiones() {
	int response = 0;

	struct sockaddr_in clientAddress;
	int clientAddressLength = sizeof(clientAddress);
	char ip[INET_ADDRSTRLEN];

	while (! detenido) {
		SOCKET newSocketD = accept(socketD, (struct sockaddr*) &clientAddress,
				(socklen_t*) &clientAddressLength);

		if (newSocketD != INVALID_SOCKET) {
			inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
			sesiones.push_back(Sesion(newSocketD, ip));
		} else if (!detenido)
			response = -1;

		if (response < 0)
			detenido = true;
	}

	if (response < 0) {
		error("Error llamando a accept con código " + std::to_string(getLastError()));
	} else {
		debug("Finalizando thread aceptarConexiones");
	}
}

void Servidor::detener() {
	if (!detenido) {
		detenido = true;

		if (socketD != INVALID_SOCKET) {
			closesocket(socketD);
			debug("Socket cerrado");


			auto it = sesiones.begin();
			while (it != sesiones.end()) {
				it->detener();
				it++;
			}
			sesiones.clear();

			debug("Esperando que termine thread aceptarConexiones");
			thread.join();
			debug("Thread aceptarConexiones termino");
		}
		info("Servidor detenido");
	}
}

