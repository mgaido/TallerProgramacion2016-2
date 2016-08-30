#include "Servidor.h"

Servidor::Servidor() {
	socketD = INVALID_SOCKET;
	stop = false;
}

Servidor::~Servidor() {
	detener();
}

void Servidor::iniciar(int port) {
	std::cout << "Precioná enter para terminar..." << std::endl;
	listenThread = std::thread(&Servidor::listenLoop, this, port);
	std::string msg;
	std::getline(std::cin, msg);
	detener();
}

void Servidor::listenLoop(int port) {
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
		response = bind(socketD, (struct sockaddr*) &serverAddress,
				sizeof(serverAddress));
		if (response == 0) {
			fase = "listen";
			response = listen(socketD, 4);
			if (response == 0) {
				fase = "poll";
				while (!stop) {
					struct pollfd pollD[1];
					pollD[0].fd = socketD;
					pollD[0].events = POLLIN;
					response = poll(pollD, 1, 1000);

					if (response > 0) {
						struct sockaddr_in clientAddress;
						int clientAddressLength = sizeof(clientAddress);

						fase = "poll";
						SOCKET newSocketD = accept(socketD,
								(struct sockaddr*) &clientAddress,
								(socklen_t*) &clientAddressLength);

						char ip[INET_ADDRSTRLEN];
						inet_ntop(AF_INET, &(clientAddress.sin_addr), ip,
								INET_ADDRSTRLEN);

						handlers.push_back(
								std::thread(&Servidor::handleClient, this,
										newSocketD, std::string(ip)));
					}
					if (response < 0)
						stop = true;
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
	if (! stop) {
		stop = true;

		auto it = handlers.begin();
		while (it != handlers.end()) {
			it->join();
			it++;
		}
		handlers.clear();
		listenThread.join();
	}
}

void Servidor::handleClient(SOCKET newSocketD, std::string clientIp) {
	if (newSocketD != INVALID_SOCKET) {
		Conexion con(newSocketD);
		std::string text = con.recibir();
		std::cout << "El cliente " << clientIp << " envió " << text << std::endl;
		con.enviar("Recibido: \"" + text + "\"");
	}
}
