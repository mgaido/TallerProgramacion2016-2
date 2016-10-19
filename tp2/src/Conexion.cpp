#include "Conexion.h"

const int HSK_REQ = 0;
const int HSK_RES = 1;
const int KEY = 2;
const int UPD = 3;
const int BYE = 4;
const int RLD = 5;

Conexion::Conexion() {
	this->socketD = INVALID_SOCKET;
	puedeRecibir = true;
}

Conexion::~Conexion() {
}

void Conexion::setSocket(SOCKET socketD) {
	this->socketD = socketD;
}

void Conexion::enviar(Bytes &mensaje) {
	std::unique_lock<std::mutex> lock(sendMutex);
	
	int size = mensaje.size();
	int sent = send(socketD, (char*)&size, sizeof(size), 0);
	if (sent == sizeof(size)) {
		const char* c = mensaje.toVector().data();
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

Bytes Conexion::recibir() {
	std::unique_lock<std::mutex> lock(recvMutex);

	struct pollfd pollD[1];
	pollD[0].fd = socketD;
	pollD[0].events = POLLIN;
	int response = 0;
	do {
		response = poll(pollD, 1, 500);
	} while (response == 0 && puedeRecibir && socketD != INVALID_SOCKET);

	if (response > 0 && puedeRecibir) {
		Bytes mensaje;

		int size;
		int read = recv(socketD, (char*) &size, sizeof(size), 0);
		std::vector<char> bytes;
		bytes.reserve(size);
		bytes.resize(size);

		if (read == sizeof(size)) {
			read = 0;
			while (read < size) {
				int r = recv(socketD, bytes.data() + read, size - read, 0);
				if (r > 0)
					read += r;
				else {
					error("Error leyendo mensaje del socket. Codigo " + std::to_string(getLastError()));
					throw SocketException();
				}
			}
			mensaje.fromVector(bytes);
			return mensaje;
		} else {
			error("Error leyendo la longitud del mensaje del socket. Codigo " + std::to_string(getLastError()));
			throw SocketException();
		}
	}

	if (puedeRecibir && response < 0) {
		error("Error esperando datos para lectura. Codigo " + std::to_string(getLastError()));
	} else
		debug("No se puede recibir: conexion cerrada");

	throw SocketException();
}

void Conexion::cancelarRecepcion() {
	puedeRecibir = false;
}

void Conexion::habilitarRecepcion() {
	puedeRecibir = true;
}

bool Conexion::getPuedeRecibir() {
	return puedeRecibir;
}

void Conexion::cerrar() {
	if (socketD != INVALID_SOCKET) {
		closesocket(socketD);
		info("Socket " + std::to_string(socketD) + " cerrado");
		socketD = INVALID_SOCKET;
	}
}

