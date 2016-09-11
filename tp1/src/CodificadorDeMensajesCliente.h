#ifndef CODIFICADOR_DE_MENSAJES_CLIENTE
#define CODIFICADOR_DE_MENSAJES_CLIENTE

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajesCliente {
public:
	CodificadorDeMensajesCliente(Conexion *nuevaCon);
	~CodificadorDeMensajesCliente();
	void enviarMensajeFormateado(std::string destinatario, std::string texto);
	void recibirMensajes();

private:
	void clrScrn();
	Conexion *con;
};

#endif
