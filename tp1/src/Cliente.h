#ifndef CLIENTE_H
#define CLIENTE_H

#include "CodificadorDeMensajes.h"
#include "stdafx.h"
#include "Conexion.h"

class Cliente {
public:
	Cliente();
	void conectar(std::string host, int puerto);
	void desconectar();
private:
	void leerComando();
	void enviarMensaje(Conexion con);
	void recibirMensaje(Conexion con);
	void loremIpsum(Conexion con);
	void loguear(Conexion con);
	void parseoUsuario(std::string textoUsuarios);
	char imprimirMenu();

	std::string host;
	int puerto;
	SOCKET socketD;
	bool conectado;
	bool logueado;
	std::vector<std::string> usuarios;
};

#endif // CLIENTE_H
