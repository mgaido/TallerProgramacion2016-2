#include "../stdafx.h"
#include "servidor.h"
#include "cliente.h"
#include <iostream>

int main(int argc, char *argv[])
{

	bool server = true;
	std::string host;
	int puerto = -1;

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
						auto sPuerto = std::string(argv[++i]);
						puerto = std::stoi(sPuerto);
					}
					break;
				default: break;
				}
			}
		}
	}

	std::cout << (server ? "Servidor" : "Cliente") << " puerto " << puerto << std::endl;
	if (puerto < 0)
		return -1;

	if (server) {
		Servidor servidor;
		servidor.iniciar(puerto);
	}
	else {
		if (host.length() > 0) {
			Cliente cliente;
			cliente.conectar(host, puerto);
			if (cliente.estaConectado())
				cliente.desconectar();
		}
	}


	return 0;
}
