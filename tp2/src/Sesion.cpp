/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip, Juego* juego) {
	this->ip = ip;
	this->detenido=false;
	this->juego = juego;
	this->jugador = nullptr;

	info("Cliente " + ip + " conectado.");
	con.setSocket(socketD);

	this->t_events = std::thread(&Sesion::atenderCliente, this);
	this->t_updates = std::thread(&Sesion::enviarActializaciones, this);
}

Sesion::~Sesion(){
	t_events.join();
	t_updates.join();
}

void Sesion::nuevaActualizacion(Bytes bytes) {
	actualizaciones.encolar(bytes);
}

void Sesion::atenderCliente() {
	//TODO handshake
	jugador = juego->nuevoJugador(ip);

	while (!detenido) {
		try {
			Bytes bytes = con.recibir();

			if ( bytes.size() > 0) {
				int comando;
				bytes.get(comando);
				if (comando == KEY) {
					eventoTeclado(bytes);
				} else if (comando == BYE) {
					desconectar();
				} else {
					warn("Comando invalido: " + std::to_string(comando));
				}
			}
		} catch (SocketException&) {
			if (!detenido) {
				error("Error de conexion con " + ip);
				detenido = true;
			}
		}
	}
	detener();
}

void Sesion::eventoTeclado(Bytes& bytes) {
	int estado=0;
	bytes.get(estado);
	Teclas teclas;
	teclas.setEstado(estado);

	if (teclas.arriba())
		jugador->saltar();
	else if (teclas.der() && ! teclas.izq())
		jugador->caminar(Direccion::DERECHA);
	else if (teclas.izq() && ! teclas.der())
		jugador->caminar(Direccion::IZQUIERDA);
	else
		jugador->detenerse();

}

void Sesion::enviarActializaciones() {
	while (! detenido) {
		try {
			Bytes bytes = actualizaciones.desencolar();
			con.enviar(bytes);
		} catch (ColaCerrada&) {
			break;
		}
	}
}


void Sesion::detener() {
	detenido = true;
	con.cerrar();
	actualizaciones.cerrar();
}

void Sesion::desconectar() {
	detenido = true;
	info("Cliente " + ip + " desconectado.");
}
