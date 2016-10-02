/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip) {
	this->ip = ip;
	this->detenido=false;
	this->thread = std::thread(&Sesion::atenderCliente, this, socketD);
}

Sesion::~Sesion(){
	thread.join();
}

void Sesion::detener() {
	detenido = true;
	con.cerrar();
}

void Sesion::atenderCliente(SOCKET socketD) {
	info("Cliente " + ip + " conectado.");
	con.setSocket(socketD);
	while (!detenido) {
		try {
			Bytes bytes = con.recibir();
			if ( bytes.size() > 0) {
				int comando;
				bytes.get(comando);

				if (comando == KEY) {
					int estado;
					bytes.get(estado);
					Teclas teclas;
					teclas.setEstado(estado);
					eventoTeclado(teclas);
				} else if (comando == BYE) {
					desconectar();
				} else {
					warn("Comando invalido: " + std::to_string(comando));
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

void Sesion::eventoTeclado(Teclas& teclas) {
	std::stringstream ss;
	ss << "Teclas: " << teclas.getEstado();
	if (teclas.arriba())
		ss << " Arriba";
	if (teclas.izq())
		ss << " Izquierda";
	if (teclas.der())
		ss << " Derecha";

	info(ss.str(), true);
}

void Sesion::desconectar() {
	detenido = true;
	try {
		//con.enviar(BYE);
	} catch (SocketException) {
		warn("Error al desconectar");
	}
	info("Cliente " + ip + " desconectado.");
}
