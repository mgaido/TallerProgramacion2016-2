#ifndef CONEXION_H
#define CONEXION_H

#include "stdafx.h"
#include "SocketException.h"

using bytes = std::vector<char>;

class Conexion {
public:
	Conexion(SOCKET socketD);
	Conexion();

	void enviar(std::string mensaje);
	std::string recibir();
private:
	SOCKET socketD;
};

#endif // CONEXION_H

