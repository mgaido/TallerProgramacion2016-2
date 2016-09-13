#ifndef CLIENTE_H
#define CLIENTE_H

#include "stdafx.h"
#include "Conexion.h"
#define RANGO_LONGITUD_MENSAJE_LOREM_IPSUM 200

class Cliente {
public:
	Cliente(std::string host,int puerto);
	void conectar();
	void desconectar();
	void iniciar();
private:
	void enviarMensaje();
	void enviarMensaje(int usuario, std::string texto);
	void recibirMensajes();
	void loremIpsum();
	void loguear();
	void parseoUsuario(std::string textoUsuarios);
	char imprimirMenu();
	std::string getMensajeLoremIpsum(std::ifstream &archivo, int longitudMensaje);

	std::string host;
	int puerto;
	SOCKET socketD;
	Conexion con;
	bool conectado;
	bool logueado;
	std::vector<std::string> usuarios;
};

#endif // CLIENTE_H
