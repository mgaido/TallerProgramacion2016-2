#ifndef CLIENTE_H
#define CLIENTE_H

#include "stdafx.h"
#include "Conexion.h"

class Cliente {
public:
	Cliente();
	void conectar(std::string host, int puerto);
	void desconectar();
private:
	void leerComando();

	std::string host;
	int puerto;
	SOCKET socketD;
	bool conectado;
	bool logueado;
	char imprimirMenu();
	void loguear(Conexion con);
	void parseoUsuario(std::string textoUsuarios);
	std::vector<std::string> usuarios;
};

#endif // CLIENTE_H
