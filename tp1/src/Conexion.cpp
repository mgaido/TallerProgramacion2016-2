#include "Conexion.h"

Conexion::Conexion(SOCKET socketD) {
	this->socketD = socketD;
}

Conexion::Conexion() {
	
}

void Conexion::enviar(std::string mensaje) throw(SocketException) {
	int size = mensaje.size();
	if (send(socketD, (char*) &size, sizeof(size), 0) != SOCKET_ERROR) {
		if (send(socketD, mensaje.data(), size, 0) == SOCKET_ERROR) {
			throw SocketException("Error escribiendo el mensaje al socket. Código "
									+ std::to_string(getLastError()));
		}
	} else {
		throw SocketException("Error escribiendo la longitud del mensaje al socket. Código "
								+ std::to_string(getLastError()));
	}
}

std::string Conexion::recibir() throw(SocketException) {
	int size;
	std::string mensaje;

	int read = recv(socketD, (char*) &size, sizeof(size), 0);
	if (read == sizeof(size)) {
		char* c = new char[size];
		read = 0;
		while (read < size) {
			int r = recv(socketD, c + read, size - read, 0);
			if (r > 0)
				read += r;
			else
				throw SocketException("Error leyendo el mensaje el socket. Código "
						+ std::to_string(getLastError()));
		}
		mensaje.assign(c, size);
		delete c;
	} else {
		throw SocketException("Error leyendo la longitud del mensaje del socket. Código "
						+ std::to_string(getLastError()));
	}

	return mensaje;
}
