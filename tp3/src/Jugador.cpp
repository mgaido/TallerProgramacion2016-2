/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

Jugador::Jugador(int id, std::string nombre, Config& _configuracion) : Personaje(id, _configuracion) {
	this->nombre = nombre;
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
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

void Jugador::recibirBonus(PickUp* unPickUp) {
	switch ((int)unPickUp->getBonus()) {
	case (int)Bonus::GunH: {
		if (unPickUp->getBonus() != (Bonus)arma->getTipo())
			this->arma = new GunH(++contador);
		else
			this->arma->recargar();
		break;
	}
	case (int)Bonus::GunC: {
		if (unPickUp->getBonus() != (Bonus) arma->getTipo())
			this->arma = new GunC(++contador);
		else
			this->arma->recargar();
		break;
	}
	case (int)Bonus::GunS: {
		if (unPickUp->getBonus() != (Bonus)arma->getTipo())
			this->arma = new GunS(++contador);
		else
			this->arma->recargar();
		break;
	}
	case (int)Bonus::GunR: {
		if (unPickUp->getBonus() != (Bonus)arma->getTipo())
			this->arma = new GunR(++contador);
		else
			this->arma->recargar();
		break;
	}
	case (int)Bonus::Vida: {
		if (energia + unPickUp->getEnergiaACurar() > 1000) {
			energia = 1000;
		}
		else {
			energia += unPickUp->getEnergiaACurar();
		}
		break;
	}
	case(int)Bonus::KillAll: {
		killAll = true;
		break;
	}
	}
}

void Jugador::cambiarArma(Proyectil *armaNueva) {
	arma = armaNueva;
}

bool Jugador::getKillAll() {
	return killAll;
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

	if (estado != Estado::Desconectado && !conectado) {
		estado = Estado::Desconectado;
		frame = 0;
		cambios = true;
	}
}