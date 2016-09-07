#ifndef CODIFICADOR_DE_MENSAJES_CLIENTE
#define CODIFICADOR_DE_MENSAJES_CLIENTE

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajesCliente {
public:
	CodificadorDeMensajesCliente(SOCKET socketD);
	~CodificadorDeMensajesCliente();
	void enviarMensajeFormateado(std::string destinatario, std::string texto);
	void recibirMensajes();

private:
	Conexion con;
};

#endif
