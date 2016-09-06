#ifndef CODIFICADOR_DE_MENSAJES
#define CODIFICADOR_DE_MENSAJES

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajes {
public:
	CodificadorDeMensajes(Conexion nuevaConexion);

private:
	Conexion con;
	
	void enviarMensajeFormateado(std::string destinatario, std::string texto);
};

#endif
