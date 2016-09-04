#include "Cliente.h"

Cliente::Cliente() {
	connected = false;
	logueado = false;
	socketD = INVALID_SOCKET;
}

void Cliente::loguear() {
	std::cout << "Ingrese Usuario: ";
	std::string usuario;
	std::getline(std::cin, usuario);
	std::cout << "Ingrese Clave: ";
	std::string clave;
	std::getline(std::cin, clave);

	Conexion con(socketD);
	con.enviar(usuario+","+clave);

	std::string resp = con.recibir();
	std::cout << "El servidor respondió: " << resp << std::endl;
}

void Cliente::conectar(std::string host, int puerto) {
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
				std::cout << "Conectado a " << host << ":" << puerto << std::endl;
				if (logueado) {
					std::cout << "Conectado a " << host << ":" << puerto << ". Escribir mensaje: ";

					std::string msg;
					std::getline(std::cin, msg);

					Conexion con(socketD);
					con.enviar(msg);

					std::string resp = con.recibir();
					std::cout << "El servidor respondió: " << resp << std::endl;
				}
				else {
					loguear();
				}
				
			}
		}
	}

	if (response < 0) {
		std::cerr << "Error at " << fase << " " << getLastError() << std::endl;
	}
}

bool Cliente::estaConectado()
{
	return connected;
}

void Cliente::desconectar()
{
	if (connected)
#ifdef __linux__
		shutdown(socketD, 2);
#elif _WIN32
		closesocket(socketD);
#endif
	connected = false;
}
