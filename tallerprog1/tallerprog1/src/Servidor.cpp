#include "Servidor.h"

Servidor::Servidor() {
	stop = false;
}

void Servidor::iniciar(int port) {
	int response = -1;

	std::string fase = "socket";
	socketD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketD != INVALID_SOCKET) {
		response = 0;
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(port);

		fase = "bind";
		response = bind(socketD, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
		if (response == 0) {
			fase = "listen";
			response = listen(socketD, 4);
			if (response == 0) {
				fase = "accept";
				while (!stop) {
					struct sockaddr_in clientAddress;
					int clientAddressLength = sizeof(clientAddress);

					SOCKET newSocketD = accept(socketD, (struct sockaddr*) &clientAddress, (socklen_t*)&clientAddressLength);
					if (newSocketD != INVALID_SOCKET)
						this->handleClient(newSocketD, clientAddress);
					else {
						response = -1;
						stop = true;
					}
				}
#ifdef __linux__
				shutdown(socketD, 2);
#elif _WIN32
				closesocket(socketD);
#endif
			}
		}
	}

	if (response < 0) {
		std::cerr << "Error at " << fase << " " << getLastError() << std::endl;
	}

}

void Servidor::detener() {
	stop = true;
}

void Servidor::handleClient(SOCKET newSocketD, sockaddr_in clientAddress) {
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);

	Conexion con(newSocketD);
	std::string text = con.recibir();
	std::cout << "El cliente " << ip << " envió " << text << std::endl;
	std::cout << "Escribir respuesta: ";

	std::string msg;
	std::getline(std::cin, msg);
	con.enviar(msg);
	detener();
}
