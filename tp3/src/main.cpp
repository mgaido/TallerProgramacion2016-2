#include "Logger.h"

#include "Servidor.h"
#include "Cliente.h"

Logger* logger;

#undef main

int main(int argc, char *argv[]) {

	bool server = false;
	bool client = false;

	std::string host = "127.0.0.1";
	std::string usuario = "player1";
	std::string archivos = "nivel1.xml,nivel2.xml,nivel3.xml";
	int puerto = 10000;

	bool inmortal = false;
	char modo = MODO_INDIVIDUAL;

	for (int i = 1; i < argc; ++i) {
		auto param = std::string(argv[i]);
		if (param[0] == '-') {
			if (param.length() == 2) {
				switch (param[1]) {
				case 'h':
					if (server) {
						error("Opcion incompatible con modo servidor: " + param[1], true);
					} else {
						client = true;
						if (i + 1 < argc)
							host = std::string(argv[++i]);
					}
					break;
				case 'p':
					if (i + 1 < argc) {
						std::string sPuerto = std::string(argv[++i]);
						puerto = std::stoi(sPuerto);
					}
					break;
				case 'u':
					if (server) {
						error("Opcion incompatible con modo servidor: " + param[1], true);
					} else {
						client = true;
						if (i + 1 < argc)
							usuario = std::string(argv[++i]);
					}
					break;
				case 'e':
					if (client) {
						error("Opcion incompatible con modo cliente: " + param[1], true);
					} else {
						server = true;
						if (i + 1 < argc)
							archivos = std::string(argv[++i]);
					}
					break;
				case 'i':
					if (client) {
						error("Opcion incompatible con modo cliente: " + param[1], true);
					} else {
						server = true;
						inmortal = true;
					}
					break;
				case 'm':
					if (client) {
						error("Opcion incompatible con modo cliente: " + param[1], true);
					} else {
						server = true;
						if (i + 1 < argc) {
							std::string valor = std::string(argv[++i]);
							char cvalor = (char) std::stoi(valor);
							if (cvalor == MODO_COOP || cvalor == MODO_GRUPAL || cvalor == MODO_INDIVIDUAL)
								modo = cvalor;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if (! server && ! client)
		server = true;

	initSockets();

	if (server) {
		logger = new Logger("servidor.log");
		info("Iniciado servidor en puerto " + std::to_string(puerto) + (archivos.size() == 0 ? "" : " y leyendo configuracion de " + archivos), true);

		Servidor servidor(puerto, archivos, inmortal, modo);
		servidor.iniciar();
		info("Servidor finalizado");
	}

	if (client) {
		logger = new Logger(usuario + ".log");
		info("Iniciando cliente - Se conectara a: " + host + ':'+ std::to_string(puerto), true);

		Cliente cliente(host, puerto, usuario);
		cliente.iniciar();
		info("Cliente finalizado");
	}

	delete logger;

	return 0;
}

