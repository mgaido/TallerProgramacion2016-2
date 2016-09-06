#ifndef CODIFICADOR_DE_MENSAJES_SERVIDOR
#define CODIFICADOR_DE_MENSAJES_SERVIDOR

#include "stdafx.h"
#include "Conexion.h"

class CodificadorDeMensajesServidor {
public:
	CodificadorDeMensajesServidor(SOCKET socketD);
	~CodificadorDeMensajesServidor();
	void interpretarComando(std::string);

private:
	Conexion *con;
};

#endif
