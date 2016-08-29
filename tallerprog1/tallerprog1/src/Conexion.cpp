#include "Conexion.h"

Conexion::Conexion(SOCKET socketD)
{
	this->socketD = socketD;
}

void Conexion::enviar(std::string mensaje)
{
	int size = mensaje.size();
	bool ok = false;
	if (send(socketD, (char*)&size, sizeof(size), 0) != SOCKET_ERROR) {
		ok = send(socketD, mensaje.data(), size, 0) != SOCKET_ERROR;
	}

	if (!ok) {
		std::cerr << "Error sending " << getLastError() << std::endl;
	}
}

std::string Conexion::recibir()
{
	int size;
	std::string mensaje;

	bool ok = false;
	if (recv(socketD, (char*)&size, sizeof(size), 0) != SOCKET_ERROR) {
		char* c = new char[size];
		int read = 0;
		while (read < size) {
			int r = recv(socketD, c + read, size - read, 0);
			if (r > 0)
				read += r;
			else {
				ok = false;
				break;
			}
			ok = true;
		}
		if (ok)
			mensaje.assign(c, size);
		delete c;
	}

	if (!ok)
		std::cerr << "Error receiving " << getLastError() << std::endl;

	return mensaje;
}

