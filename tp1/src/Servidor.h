#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "stdafx.h"
#include "Conexion.h"

class Servidor {

public:
	Servidor();
	~Servidor();

	void iniciar(int puerto);
	void detener();
private:
	SOCKET socketD;
	bool stop;
	std::thread listenThread;
	std::vector<std::thread> handlers;

	void listenLoop(int port);
	void handleClient(SOCKET newSocketD, std::string clientIp);
};

#endif // SERVIDOR_H
