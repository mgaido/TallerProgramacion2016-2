#ifndef CODIFICADOR_DE_MENSAJES
#define CODIFICADOR_DE_MENSAJES

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajes {
public:
	CodificadorDeMensajes(Conexion nuevaConexion);
	void enviarMensajeFormateado(std::string destinatario, std::string texto);

private:
	Conexion con;
	
};

#endif