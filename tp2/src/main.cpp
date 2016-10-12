#include "Logger.h"

#include "Servidor.h"
#include "Cliente.h"

Logger* logger;

#undef main

int main(int argc, char *argv[]) {
	bool client = true;
	bool server = true;
	std::string host = "127.0.0.1";
	std::string usuario = "player1";
	std::string archivo = "config.xml";
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
						usuario = std::string(argv[++i]);
					}
				case 'c':
					if (i + 1 < argc) {
						archivo = std::string(argv[++i]);
					}
					break;
				case 's':
					client = false;
					break;
				default:
					break;
				}
			}
		}
	}

	initSockets();
	logger = new Logger("tp2.log");
	{
		Servidor servidor(puerto, archivo);
		if (server) {
			std::cout << "> Iniciado servidor en puerto " << std::to_string(puerto) << " y leyendo configuracion de " << archivo << std::endl;
			servidor.iniciar();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		} 

		if (client) {
			std::cout << "> Iniciando cliente - Se conectara a: " << host << ':' << std::to_string(puerto) << std::endl;
			Cliente cliente(host, puerto, usuario);
			cliente.iniciar();
			cliente.desconectar();
			debug("> Cliente finalizado", true);
		}

		if (server) {
			if (! client) {
				std::cout << "Presione enter para detener el servidor" << std::endl;
				std::string s;
				getline(std::cin, s);
			}

			servidor.detener();
			debug("> Servidor finalizado", true);
		}
	}

	delete logger;

	return 0;
}

