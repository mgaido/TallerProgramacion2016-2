#pragma once

#include "winsock2.h"
#include "ws2tcpip.h"

#include <string>
#include <iostream>
#include <vector>

using bytes = std::vector<char>;

class Conexion
{
public:
	Conexion(SOCKET socketD);

	void enviar(std::string mensaje);
	std::string recibir();
private:
	SOCKET socketD;
};

