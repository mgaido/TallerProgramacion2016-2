#include "Logger.h"

#include "Servidor.h"
#include "Cliente.h"

Logger* logger;

#undef main


int both() {
	std::string host = "127.0.0.1";
	std::string usuario = "Jugador 1";
	std::string archivo = "menu.xml";
	int puerto = 10000;

	initSockets();

	logger = new Logger("tp3.log");

	info("Iniciado servidor en puerto " + std::to_string(puerto) + (archivo.size() == 0 ? "" : " y leyendo configuracion de " + archivo), true);
	Servidor servidor(puerto, archivo);
	servidor.iniciar();

	info("Iniciando cliente - Se conectara a: " + host + ':'+ std::to_string(puerto), true);
	Cliente cliente(host, puerto, usuario);
	cliente.iniciar();

	cliente.desconectar();
	info("Cliente finalizado", true);

	servidor.detener();
	info("Servidor finalizado", true);

	delete logger;

	return 0;
}

int main(int argc, char *argv[]) {

	if (argc == 1)
		return both();

	bool server = false;
	bool client = false;

	std::string host = "127.0.0.1";
	std::string usuario = "player1";
	std::string archivo;
	int puerto = 10000;

	for (int i = 1; i < argc; ++i) {
		auto param = std::string(argv[i]);
		if (param[0] == '-') {
			if (param.length() == 2) {
				switch (param[1]) {
				case 'h':
					if (server) {
						error("Opcion incompatible con modo servidor", true);
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
						error("Opcion incompatible con modo servidor", true);
					} else {
						client = true;
						if (i + 1 < argc)
							usuario = std::string(argv[++i]);
					}
					break;
				case 'e':
					if (client) {
						error("Opcion incompatible con modo cliente", true);
					} else {
						server = true;
						if (i + 1 < argc)
							archivo = std::string(argv[++i]);
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
		info("Iniciado servidor en puerto " + std::to_string(puerto) + (archivo.size() == 0 ? "" : " y leyendo configuracion de " + archivo), true);

		Servidor servidor(puerto, archivo);
		servidor.iniciar();

		std::cout << "Presione enter para detener el servidor" << std::endl;
		std::string s;
		getline(std::cin, s);

		servidor.detener();
		info("Servidor finalizado", true);
	}


	if (client) {
		logger = new Logger(usuario + ".log");
		info("Iniciando cliente - Se conectara a: " + host + ':'+ std::to_string(puerto), true);

		Cliente cliente(host, puerto, usuario);
		cliente.iniciar();

		cliente.desconectar();
		info("Cliente finalizado", true);
	}

	delete logger;

	return 0;
}

