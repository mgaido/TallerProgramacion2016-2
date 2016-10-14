#ifndef CONEXION_H
#define CONEXION_H

#include "Bytes.h"
#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

extern const int KEY;
extern const int HSK_REQ;
extern const int HSK_RES;
extern const int KEY;
extern const int INIT;
extern const int UPD;
extern const int BYE;

class Conexion {
public:
	Conexion();
	~Conexion();
	void setSocket(SOCKET socketD);
	void enviar(Bytes &msg);
	Bytes recibir();

	void cancelarRecepcion();
	void cerrar();
private:
	SOCKET socketD;
	bool puedeRecibir;
	std::mutex sendMutex;
	std::mutex recvMutex;
};

#endif // CONEXION_H

