#ifndef CONEXION_H
#define CONEXION_H

#include "Bytes.h"
#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

extern const int HSK_REQ;
extern const int HSK_RES;
extern const int KEY;
extern const int RLD;
extern const int UPD;
extern const int BYE;

class Conexion {
public:
	Conexion();
	~Conexion();
	void setSocket(SOCKET socketD);
	void enviar(Bytes &msg);
	Bytes recibir();

	void habilitarRecepcion();
	void cancelarRecepcion();
	bool getPuedeRecibir();

	void cerrar();
private:
	SOCKET socketD;
	bool puedeRecibir;
	std::mutex sendMutex;
	std::mutex recvMutex;
};

#endif // CONEXION_H

