#include "../stdafx.h"
#include "Conexion.h"

Conexion::Conexion(SOCKET socketD)
{
	this->socketD = socketD;
}

void Conexion::enviar(std::string mensaje)
{
	int size = mensaje.size();
	int result;
	if (send(socketD, (char*)&size, sizeof(size), 0)) {
		result = send(socketD, mensaje.data(), size, 0);
	}
	else
		result = -1;

	if (result < 0) {
		std::cerr << "Error sending " << WSAGetLastError() << std::endl;
	}
}

std::string Conexion::recibir()
{
	int size;
	std::string mensaje;

	int result;
	if (recv(socketD, (char*)&size, sizeof(size), 0)) {
		char* c = new char[size];
		int read = 0;
		while (read < size) {
			int r = recv(socketD, c + read, size - read, 0);
			if (r > 0)
				read += r;
			else {
				result = -1;
				break;
			}
		}
		if (result > 0) {
			mensaje.assign(c, size);
		}
		delete c;
	}
	else
		result = -1;

	if (result < 0)
		std::cerr << "Error receiving " << WSAGetLastError() << std::endl;

	return mensaje;
}

