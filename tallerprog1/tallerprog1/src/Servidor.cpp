#include "../stdafx.h"
#include "servidor.h"

Servidor::Servidor() {
	stop = false;
}

void Servidor::iniciar(int port) {
	int response = -1;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

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

					SOCKET newSocketD = accept(socketD, (struct sockaddr*) &clientAddress, &clientAddressLength);
					if (newSocketD != INVALID_SOCKET)
						this->handleClient(newSocketD, clientAddress);
					else {
						response = -1;
						stop = true;
					}
				}
			}
		}
	}

	if (response < 0) {
		std::cerr << "Error at " << fase << " " << WSAGetLastError() << std::endl;
	}

}

void Servidor::detener() {
	stop = true;
}

void Servidor::handleClient(SOCKET newSocketD, sockaddr_in clientAddress) {
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
	std::cout << "Accepted connection from " << ip << std::endl;

	Conexion con(newSocketD);
	std::string text = con.recibir();
	std::cout << text << std::endl;

	con.enviar("ack");
	detener();
}
