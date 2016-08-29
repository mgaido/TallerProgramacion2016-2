#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "winsock2.h"
#include "ws2tcpip.h"

#include <string>
#include <iostream>

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
