#include "Conexion.h"

Conexion::Conexion(SOCKET socketD) {
	this->socketD = socketD;
}

Conexion::Conexion() {
	
}

void Conexion::enviar(std::string mensaje) {
	mensaje += DELIM;
	int size = mensaje.size();

	int sent = send(socketD, (char*)&size, sizeof(size), 0);
	if (sent == sizeof(size)) {
		const char* c = mensaje.c_str();
		sent = 0;
		while (sent < size) {
			int s = send(socketD, (char*) c + sent, size - sent, 0);
			if (s > 0)
				sent += s;
			else {
				error("Error leyendo mensaje del socket. Codigo " + std::to_string(getLastError()));
				throw SocketException();
			}
		}
	} else {
		error("Error escribiendo la longitud del mensaje del socket. Codigo " + std::to_string(getLastError()));
		throw SocketException();
	}
}

std::string Conexion::recibir() {
	int size;
	std::string mensaje;

	int read = recv(socketD, (char*) &size, sizeof(size), 0);
	if (read == sizeof(size)) {
		std::unique_ptr<char[]> buffer(new char [size]);
		read = 0;
		while (read < size) {
			int r = recv(socketD, buffer.get() + read, size - read, 0);
			if (r > 0)
				read += r;
			else {
				error("Error leyendo mensaje del socket. Codigo " + std::to_string(getLastError()));
				throw SocketException();
			}
		}
		mensaje.assign(buffer.get(), size);
	} else {
		error("Error leyendo la longitud del mensaje del socket. Codigo " + std::to_string(getLastError()));
		throw SocketException();
	}
	return mensaje;
}
