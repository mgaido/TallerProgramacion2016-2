#ifndef CLIENTE_H
#define CLIENTE_H

#include "CodificadorDeMensajes.h"
#include "stdafx.h"
#include "Conexion.h"

class Cliente {
public:
	Cliente(std::string host,int puerto);
	void conectar();
	void desconectar();
	void iniciar();
private:
	void leerComando();
	void enviarMensaje();
	void recibirMensajes();
	void loremIpsum();
	void loguear(Conexion con);
	void parseoUsuario(std::string textoUsuarios);
	char imprimirMenu();

	std::string host;
	int puerto;
	SOCKET socketD;
	Conexion con;
	bool conectado;
	bool logueado;
	std::vector<std::string> usuarios;
};

#endif // CLIENTE_H
