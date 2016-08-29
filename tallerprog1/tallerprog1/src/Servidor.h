#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "stdafx.h"
#include "Conexion.h"

class Servidor {

public:
	Servidor();

	void iniciar(int puerto);
	void detener();
private:
	SOCKET socketD;
	bool stop;
	void handleClient(SOCKET newSocketD, sockaddr_in clientAddress);
};

#endif // SERVIDOR_H
