#ifndef CONEXION_H
#define CONEXION_H

#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

#define DELIM '\0'

#define AUTH "AUTH"
#define SEND "SEND"
#define RETR "RETR"
#define PING "PING"
#define BYE "BYE"

#define SUCCESS "SUCCESS"
#define FAIL "FAIL"

class Conexion {
public:
	Conexion();
	void setSocket(SOCKET socketD);
	void enviar(std::string mensaje);
	std::string recibir();
private:
	SOCKET socketD;
};

#endif // CONEXION_H

