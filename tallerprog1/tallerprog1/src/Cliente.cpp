#include "../stdafx.h"
#include "cliente.h"

Cliente::Cliente()
{
}


void Cliente::conectar(std::string host, int puerto) {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	std::string fase = "socket";
	int response = -1;
	socketD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketD != INVALID_SOCKET) {
		response = 0;
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(puerto);

		fase = "inet_pton";
		response = inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr.s_addr));
		if (--response == 0) {
			fase = "connect";
			response = connect(socketD, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
			if (response == 0) {
				connected = true;
				std::cout << "Connected to " << host << ":" << puerto << std::endl;
				std::cout << "Escribir mensaje: ";

				std::string msg;
				std::getline(std::cin, msg);

				Conexion con(socketD);
				con.enviar(msg);

				std::string resp = con.recibir();
				std::cout << resp << std::endl;
			}
		}
	}

	if (response < 0) {
		std::cerr << "Error at " << fase << " " << WSAGetLastError() << std::endl;
	}
}

bool Cliente::estaConectado()
{
	return connected;
}

void Cliente::desconectar()
{
	if (connected)
		closesocket(socketD);
	connected = false;
}
