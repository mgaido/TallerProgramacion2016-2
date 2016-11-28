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
	killAll = false;

	estado = Estado::Desconectado;
	tipo = Tipo::Jugador;

	cambios = false;
}

Jugador::~Jugador() {
}

std::string Jugador::getNombre() {
	return nombre;
}

void Jugador::recibirBonus(PickUp* unPickUp){
	if ((unPickUp->getArma() != NULL))
		this->arma = unPickUp->getArma();
	else if (unPickUp->getEnergiaACurar() != 0)
		energia += unPickUp->getEnergiaACurar();
	else
		killAll = true;

}

void Jugador::cambiarArma(Proyectil *armaNueva) {
	arma = armaNueva;
}

bool Jugador::esEnemigo() {
	return false;
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