/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip, Jugador* jugador) {
	this->ip = ip;
	this->activa=true;
	this->jugador = jugador;

	con.setSocket(socketD);
	this->t_atenderCliente = std::thread(&Sesion::atenderCliente, this);
	this->t_enviarActualizaciones = std::thread(&Sesion::enviarActualizaciones, this);
	jugador->setConectado(true);
}

Sesion::~Sesion(){
}

bool Sesion::estaActiva() {
	return activa;
}

Jugador* Sesion::getJugador() {
	return jugador;
}

void Sesion::nuevaActualizacion(Bytes bytes) {
	if (activa)
		actualizaciones.encolar(bytes);
}

void Sesion::atenderCliente() {
	while (activa) {
		try {
			Bytes bytes = con.recibir();
			if ( bytes.size() > 0) {
				int comando;
				bytes.get(comando);
				if (comando == KEY) {
					eventoTeclado(bytes);
				} else if (comando == INIT){
					enviarConfiguraciones();
				}
				else if (comando == BYE) {
					info("Jugador '" + jugador->getNombre() + "' desconectado desde " + ip, true);
					desconectar();
				} else {
					warn("Comando invalido: " + std::to_string(comando));
				}
			}
		} catch (SocketException&) {
			if (activa) {
				error("Error de conexion con " + ip);
				desconectar();
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

void Sesion::enviarActualizaciones() {
	while (activa) {
		try {
			Bytes bytes = actualizaciones.desencolar();
			con.enviar(bytes);
		} catch (ColaCerrada&) {
			break;
		} catch (SocketException&) {
			desconectar();
			break;
		}
	}
}

void Sesion::enviarConfiguraciones() {
	if (activa) {
		try {
			Bytes bytes;
			//creando serializacion de configuracion
			con.enviar(bytes);
		} catch (SocketException&) {
			desconectar();
		}
	}
}

void Sesion::desconectar() {
	if (activa) {
		activa = false;
		actualizaciones.cerrar();
		con.cancelarRecepcion();
		jugador->setConectado(false);
	}

	if (t_enviarActualizaciones.joinable() && t_enviarActualizaciones.get_id() != std::this_thread::get_id()) {
		debug("Esperando que termine thread enviarActualizaciones");
		t_enviarActualizaciones.join();
		debug("Thread enviarActualizaciones termino");
	}

	if (t_atenderCliente.joinable() && t_atenderCliente.get_id() != std::this_thread::get_id()) {
		debug("Esperando que termine thread atenderCliente");
		t_atenderCliente.join();
		debug("Thread atenderCliente termino");
	}

	con.cerrar();
}
