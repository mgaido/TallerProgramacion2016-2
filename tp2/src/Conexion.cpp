#include "Conexion.h"

const int KEY = 1;
const int BYE = 9;

Conexion::Conexion() {
	socketD = INVALID_SOCKET;
	sendLock = new std::mutex();
	recvLock = new std::mutex();
}

Conexion::~Conexion() {
	delete sendLock;
	delete recvLock;
}

void Conexion::setSocket(SOCKET socketD) {
	this->socketD = socketD;
}

void Conexion::enviar(Bytes &mensaje) {
	int size = mensaje.size();
	sendLock->lock();
	int sent = send(socketD, (char*)&size, sizeof(size), 0);
	if (sent == sizeof(size)) {
		const char* c = mensaje.toCharArray();
		sent = 0;
		while (sent < size) {
			int s = send(socketD, (char*) c + sent, size - sent, 0);
			if (s > 0)
				sent += s;
			else {
				sendLock->unlock();
				error("Error leyendo mensaje del socket. Codigo " + std::to_string(getLastError()));
				throw SocketException();
			}
		}
		sendLock->unlock();
	} else {
		sendLock->unlock();
		error("Error escribiendo la longitud del mensaje del socket. Codigo " + std::to_string(getLastError()));
		throw SocketException();
	}
}

Bytes Conexion::recibir() {
	int size;
	Bytes mensaje;

	recvLock->lock();

	struct pollfd pollD[1];
	pollD[0].fd = socketD;
	pollD[0].events = POLLIN;
	int response = 0;
	do {
		response = poll(pollD, 1, 250);
	} while (response == 0 && socketD != INVALID_SOCKET);

	if (response > 0) {
		int read = recv(socketD, (char*) &size, sizeof(size), 0);
		if (read == sizeof(size)) {
			std::unique_ptr<char[]> buffer(new char[size]);
			read = 0;
			while (read < size) {
				int r = recv(socketD, buffer.get() + read, size - read, 0);
				if (r > 0)
					read += r;
				else {
					recvLock->unlock();
					error("Error leyendo mensaje del socket. Codigo " + std::to_string(getLastError()));
					throw SocketException();
				}
			}
			mensaje.fromCharArray(buffer.get(), size);
			recvLock->unlock();
		} else {
			recvLock->unlock();
			error("Error leyendo la longitud del mensaje del socket. Codigo " + std::to_string(getLastError()));
			throw SocketException();
		}
	}
	return mensaje;
}

void Conexion::cerrar() {
	if (socketD != INVALID_SOCKET) {
		closesocket(socketD);
		socketD = INVALID_SOCKET;
	}
}

