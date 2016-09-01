#ifndef CONEXION_H
#define CONEXION_H

#include "stdafx.h"

using bytes = std::vector<char>;

class Conexion {
public:
	Conexion(SOCKET socketD);

	void enviar(std::string mensaje);
	std::string recibir();
private:
	SOCKET socketD;
};

#endif // CONEXION_H

