#include "Servidor.h"

Servidor::Servidor(std::string &archivo) {
	socketD = INVALID_SOCKET;
	detenido = false;
	Usuarios::leerUsuarios(archivo);
}

Servidor::~Servidor() {
	detener();
}

void Servidor::iniciar(int port) {
	thread = std::thread(&Servidor::aceptarConexiones, this, port);
	std::cout << "Preciona enter para terminar..." << std::endl;
	std::string msg;
	std::getline(std::cin, msg);
	detener();
}

void Servidor::aceptarConexiones(int port) {
	int response = -1;

	std::string funcion = "socket";
	socketD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketD != INVALID_SOCKET) {
		response = 0;

		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(port);

		funcion = "bind";
		response = bind(socketD, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
		if (response == 0) {
			funcion = "listen";
			response = listen(socketD, 4);
			if (response == 0) {
				struct sockaddr_in clientAddress;
				int clientAddressLength = sizeof(clientAddress);
				char ip[INET_ADDRSTRLEN];

				funcion = "accept";
				if (response == 0) {
					while (!detenido) {
						SOCKET newSocketD = accept(socketD, (struct sockaddr*) &clientAddress,
								(socklen_t*)&clientAddressLength);

						if (newSocketD != INVALID_SOCKET) {
							inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
							sesiones.push_back(Sesion(newSocketD, ip));
						} else if (! detenido)
							response = -1;

						if (response < 0)
							detenido = true;
					}
				}
			}
		}
	}

	if (response < 0) {
		std::string error = "Error llamando a " + funcion + " con código " + std::to_string(getLastError());
		std::cerr << error << std::endl;
	} else {
		std::cout << "Finalizando servidor" << std::endl;
	}
}

void Servidor::detener() {
	if (!detenido) {
		detenido = true;
		shutdown(socketD, 2);

		auto it = sesiones.begin();
		while (it != sesiones.end()) {
			it->detener();
			it++;
		}
		sesiones.clear();
		thread.join();
	}
}

