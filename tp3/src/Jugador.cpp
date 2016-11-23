/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

Jugador::Jugador(int id, std::string nombre, Config& _configuracion) : Personaje(id, _configuracion){
	this->nombre = nombre;
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;
	tiempoCaminando=0;
	tiempoSalto=0;

	estado = Estado::Desconectado;
	tipo = Tipo::Jugador;

	cambios = false;
}

Jugador::~Jugador() {
}

std::string Jugador::getNombre() {
	return nombre;
}


void Jugador::setConectado(bool conectado) {
	if (estado == Estado::Desconectado && conectado) {
		estado = Estado::Quieto;
		frame = 0;
		cambios = true;
	}

	if (estado != Estado::Desconectado && ! conectado) {
		estado = Estado::Desconectado;
		frame = 0;
		cambios = true;
	}
}