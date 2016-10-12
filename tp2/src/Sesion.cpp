/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip, std::string nombre,Servidor* servidor) {
	this->ip = ip;
	this->detenido=false;
	this->servidor = servidor;
	this->jugador = nullptr;
	this->nombre = nombre;


	con.setSocket(socketD);
	info("Cliente " + ip + " conectado.");
	this->t_atenderCliente = std::thread(&Sesion::atenderCliente, this);
	this->t_enviarActualizaciones = std::thread(&Sesion::enviarActializaciones, this);
}

Sesion::~Sesion(){
	debug("Esperando que termine thread atenderCliente");
	t_atenderCliente.join();
	debug("Thread atenderCliente termino");

	debug("Esperando que termine thread enviarActualizaciones");
	t_enviarActualizaciones.join();
	debug("Thread enviarActualizaciones termino");
}

void Sesion::nuevaActualizacion(Bytes bytes) {
	if (! detenido)
		actualizaciones.encolar(bytes);
}

void Sesion::atenderCliente() {
	//TODO handshake
	jugador = servidor->getJuego()->nuevoJugador(ip,nombre);
	//std::cout << nombre << std::endl;

	while (! detenido) {
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
			if (! detenido) {
				error("Error de conexion con " + ip);
				detener();
			}
		}
	}
}

void Sesion::eventoTeclado(Bytes& bytes) {
	int estado=0;
	bytes.get(estado);
	Teclas teclas;
	teclas.setEstado(estado);

	if (teclas.arriba()) {
		jugador->saltar();
		debug(ip + " tecla arriba");
	}  else if (teclas.der() && !teclas.izq()) {
		jugador->caminar(Direccion::DERECHA);
		debug(ip + " tecla derecha");
	} else if (teclas.izq() && !teclas.der()) {
		jugador->caminar(Direccion::IZQUIERDA);
		debug(ip + " tecla izquierda");
	}  else {
		jugador->detenerse();
		debug(ip + " sin teclas");
	}
}

void Sesion::enviarActializaciones() {
	while (! detenido) {
		try {
			Bytes bytes = actualizaciones.desencolar();
			con.enviar(bytes);
		} catch (ColaCerrada&) {
			break;
		} catch (SocketException&) {
			detener();
			break;
		}
	}
}

void Sesion::detener() {
	if (!detenido) {
		detenido = true;
		servidor->removerSesion(this);
		
		con.cancelarRecepcion();
		actualizaciones.cerrar();
		con.cerrar();
	}
}

void Sesion::desconectar() {
	info("Cliente " + ip + " desconectado.");
	//TODO avisarle al juego
	detener();
}
