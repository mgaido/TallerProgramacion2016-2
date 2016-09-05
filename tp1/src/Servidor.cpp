#include "Servidor.h"

Servidor::Servidor() {
	socketD = INVALID_SOCKET;
	stop = false;
	leerUsuarios();

	auto ite = usuarios.begin();
	while (ite != usuarios.end()) {
		std::cout << ite->getNombre() << "    " << ite->password << std::endl;
		ite++;

	}

}

Servidor::~Servidor() {
	detener();
}

void Servidor::leerUsuarios() {
	std::ifstream fileUsuario("arch.txt");
	std::string linea;
	std::string clave[2];
	while (fileUsuario.good() && getline(fileUsuario, linea)) {
		bool invalido = false;
		for (int i = 0; i < 2 && !invalido; i++) {
			if (linea.find(',') != std::string::npos || i == 1) {
				clave[i] = linea.substr(0, linea.find(','));
				linea = linea.substr(linea.find(',') + 1, linea.length() - 1);
			}
			else {
				invalido = true;
				std::cout << "Error" << std::endl;
			}
		}

		bool repetido = false;
		auto ite = usuarios.begin();
		while (ite != usuarios.end() && !repetido) {
			if (clave[0] == ite->getNombre()) {
				repetido = true;
			}
			ite++;
		}

		if (!invalido && clave[0] != "" && clave[1] != "" && !repetido) {
			Usuario usuario(clave[0], clave[1]);
			usuarios.push_back(usuario);
			//std::cout << "Usuario: " << clave[0] << "Clave: " << clave[1] << std::endl;
		}
	}
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
							(socklen_t*)&clientAddressLength);

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
				shutdown(socketD, 2);
			}
		}
	}

	if (response < 0) {
		std::cerr << "Error at " << fase << " " << getLastError() << std::endl;
	}
}

void Servidor::detener() {
	if (!stop) {
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


bool Servidor::autentificar(std::string msj) {
	bool correcto = false;
	if (msj.find(',') != std::string::npos) {
		std::string usuario = msj.substr(0, msj.find(','));
		std::string clave = msj.substr(msj.find(',')+1, msj.length()-1);
		auto ite = usuarios.begin();
		while (ite != usuarios.end()) {
			if (usuario == ite->getNombre()) {
				if (ite->verificarPassword(clave)) {
					correcto = true;
				}
			}
			ite++;
		}
	}
	return correcto;
	
}

void Servidor::handleClient(SOCKET newSocketD, std::string clientIp) {
	if (newSocketD != INVALID_SOCKET) {
		Conexion con(newSocketD);
		bool logueado = false;
		bool seguir = true;
		while (seguir) {
			if (logueado) {
				std::string text = con.recibir();
				std::cout << "El cliente " << clientIp << " envió " << text << std::endl;
				con.enviar("Recibido: \"" + text + "\"");
			}
			else {
				std::string text = con.recibir();
				std::cout << "El cliente " << clientIp << " envió " << text << std::endl;
				bool correcto = autentificar(text);
				if (correcto) {
					con.enviar("1-Logueo Exitoso.");
					logueado = true;
				}
				else {
					con.enviar("0-Error - Usuario o Clave Incorrecto");
					seguir = false;
				}
			}
		}
	}
}
