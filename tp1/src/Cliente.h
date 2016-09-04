#ifndef CLIENTE_H
#define CLIENTE_H

#include "stdafx.h"
#include "Conexion.h"

class Cliente {
public:
	Cliente();
	void conectar(std::string host, int puerto);
	bool estaConectado();
	void desconectar();
private:
	SOCKET socketD;
	bool connected;
	bool logueado;
	void loguear();
};

#endif // CLIENTE_H
