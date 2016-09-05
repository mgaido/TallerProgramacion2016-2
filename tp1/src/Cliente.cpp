#include "Cliente.h"

Cliente::Cliente() {
	connected = false;
	logueado = false;
	socketD = INVALID_SOCKET;
}

void Cliente::parseoUsuario(std::string textoUsuarios) {
	textoUsuarios = textoUsuarios + ",";
	while (textoUsuarios.find(',') != std::string::npos) {
		usuarios.push_back(textoUsuarios.substr(0, textoUsuarios.find(',')));
		textoUsuarios = textoUsuarios.substr(textoUsuarios.find(',') + 1, textoUsuarios.length()-1);
	}
	//for (std::vector<std::string>::const_iterator i = usuarios.begin(); i != usuarios.end(); ++i) {
	//	// process i
	//	std::cout << *i << " "; // this will print all the contents of *features*
	//}

}

void Cliente::loguear(Conexion con) {
	std::cout << "Ingrese Usuario: ";
	std::string usuario;
	std::getline(std::cin, usuario);
	std::cout << "Ingrese Clave: ";
	std::string clave;
	std::getline(std::cin, clave);


	con.enviar(usuario + "," + clave);

	std::string resp = con.recibir();
	if (resp.substr(0, resp.find('-')) == "1") {
		logueado = true;
		std::cout << "logueo correcto" << std::endl;
		parseoUsuario(resp.substr(resp.find('-')+1, resp.length()));
	}
	std::cout << "El servidor respondio: " << resp << std::endl;
}

char Cliente::imprimirMenu() {
	std::string opcion;
	char opcionChar;
	bool correcto = false;
	while (!correcto) {
		std::cout << "1. Conectar" << std::endl;
		std::cout << "2. Desconectar" << std::endl;
		std::cout << "3. Salir" << std::endl;
		std::cout << "4. Enviar" << std::endl;
		std::cout << "5. Recibir" << std::endl;
		std::cout << "6. Lorem Ipsum" << std::endl;
		std::cout << "Seleccione una opcion: ";
		std::getline(std::cin, opcion);
		
		if (opcion.length() == 1) {
			opcionChar = opcion[0];
			correcto = true;
		}
		else {
			std::cout << "Opcion Incorrecta" << std::endl;
		}
	}
	return opcionChar;
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
				Conexion con(socketD);


				std::string msg;
				std::string resp;

				while (true) {
					char opcion = imprimirMenu();
					switch (opcion) {
					case '1':
						while (!logueado) {
							loguear(con);
						}
						std::cout << "Conectado a " << host << ":" << puerto << std::endl;
						break;
					case '2':
						logueado = false;
						break;
					case '3':

						break;
					case '4':
						std::cout << "Escribir mensaje: ";
						std::getline(std::cin, msg);
						con.enviar(msg);
						
						resp = con.recibir();
						std::cout << "El servidor respondio: " << resp << std::endl;
						break;
					default:
						std::cout << "Incorrecto" << std::endl;
						break;
					}

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
