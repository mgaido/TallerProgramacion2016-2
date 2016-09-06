#ifndef CODIFICADOR_DE_MENSAJES
#define CODIFICADOR_DE_MENSAJES

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajes {
public:
	CodificadorDeMensajes(SOCKET socketD);
	~CodificadorDeMensajes();
	void enviarMensajeFormateado(std::string destinatario, std::string texto);
	void recibirMensajes();

private:
	Conexion *con;
	void parsearMensaje(std::string mensaje);

};

#endif