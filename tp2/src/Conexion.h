#ifndef CONEXION_H
#define CONEXION_H

#include "Bytes.h"
#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

extern const int KEY;
extern const int BYE;

class Conexion {
public:
	Conexion();
	~Conexion();
	void setSocket(SOCKET socketD);
	void enviar(Bytes &msg);
	Bytes recibir();

	void cerrar();
private:
	SOCKET socketD;
	std::mutex *sendLock;
	std::mutex *recvLock;
};

#endif // CONEXION_H

