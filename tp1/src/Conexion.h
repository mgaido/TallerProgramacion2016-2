#ifndef CONEXION_H
#define CONEXION_H

#include "stdafx.h"
#include "SocketException.h"

using bytes = std::vector<char>;

class Conexion {
public:
	Conexion(SOCKET socketD);
	Conexion();

	void enviar(std::string mensaje) throw(SocketException);
	std::string recibir() throw(SocketException);
private:
	SOCKET socketD;
};

#endif // CONEXION_H

