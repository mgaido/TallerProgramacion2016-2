#include "Logger.h"

#include "Servidor.h"
#include "Cliente.h"

Logger* logger;

int main(int argc, char *argv[]) {
	bool server = true;
	std::string host = "127.0.0.1";
	std::string archivo = "usuarios.txt";
	int puerto = 10000;

	for (int i = 1; i < argc; ++i) {
		auto param = std::string(argv[i]);
		if (param[0] == '-') {
			if (param.length() == 2) {
				switch (param[1]) {
				case 'h':
					server = false;
					if (i + 1 < argc)
						host = std::string(argv[++i]);
					break;
				case 'p':
					if (i + 1 < argc) {
						std::string sPuerto = std::string(argv[++i]);
						puerto = std::stoi(sPuerto);
					}
					break;
				case 'u':
					if (i + 1 < argc) {
						archivo = std::string(argv[++i]);
					}
					break;
				case 'c':
					server = false;
					break;
				default:
					break;
				}
			}
		}
	}

	#ifdef __linux__
	signal(SIGPIPE, SIG_IGN);
	#elif _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	#endif

	if (server) {
		logger = new Logger("servidor.log");
		std::cout << "Iniciado servidor en puerto " << std::to_string(puerto) << " y leyendo usuarios de " << archivo << std::endl;
		if (Usuarios::leerUsuarios(archivo)) {
			Servidor servidor(puerto);
			servidor.iniciar();
		} else
			std::cerr << "Ocurrio un problema leyendo usuarios";
	} else {
		logger = new Logger("cliente.log");
		std::cout << "Iniciando cliente - Se conectara a: " << host << ':' << std::to_string(puerto) << std::endl;
		Cliente cliente(host, puerto);
		cliente.iniciar();
	}

	delete logger;

	return 0;
}
