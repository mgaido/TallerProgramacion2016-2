/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip) {
	this->socketD = socketD;
	this->ip = ip;
	this->detenido=false;
	this->thread = std::thread(&Sesion::atenderCliente, this);
}

Sesion::~Sesion(){
	thread.join();
}

void Sesion::detener() {
	detenido = true;
	if (socketD != INVALID_SOCKET) {
		closesocket(socketD);
		socketD = INVALID_SOCKET;
	}
}

void Sesion::atenderCliente() {
	info("Cliente " + ip + " conectado. Esperando autentificacion.");

	con.setSocket(socketD);
	while (!detenido) {
		try {
			std::string text = con.recibir();
			std::vector<std::string> parametros = split(text, DELIM);

			if (! parametros.empty()) {
				std::string comando = parametros[0];
				parametros.erase(parametros.begin());

				if (comando == PING) {
					ping();
				} else if (comando == BYE) {
					desconectar();
				} else {
					warn("Comando invalido: " + comando);
					std::stringstream ss;
					ss << FAIL << DELIM << "Comando invalido: " + comando;
					con.enviar(ss.str());
				}
			}
		} catch (SocketException) {
			if (!detenido) {
				error("Error de conexion con " + ip);
				detenido = true;
			}
		}
	}
	detener();
}

void Sesion::ping() {
	con.enviar(SUCCESS);
}

void Sesion::desconectar() {
	detenido = true;
	try {
		con.enviar(BYE);
	} catch (SocketException) {
		warn("Error al desconectar");
	}
	info("Cliente " + ip + " desconectado.");
}
