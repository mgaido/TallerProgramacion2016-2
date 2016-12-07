/*
 * Arma.cpp
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#include "Arma.h"
#include "GunH.h"
#include "GunS.h"
#include "GunR.h"
#include "GunC.h"
#include "Misil.h"
#include "Bomba.h"
#include "Torreta.h"
#include "Laser.h"

Arma::Arma() {
	tipo = Tipo::Void;
	setTipo(Tipo::GunH);
}

void Arma::setTipo(Tipo tipo) {
	bool recarga = false;
	if (tipo == this->tipo)
		recarga = true;
	else
		this->tipo = tipo;

	switch (this->tipo) {
	case Tipo::GunH:
		this->balas = recarga ? balas + 150: 200;
		break;
	case Tipo::GunS:
		this->balas = recarga ? balas + 15: 30;
		break;
	case Tipo::GunR:
		this->balas = recarga ? balas + 15: 30;
		break;
	case Tipo::GunC:
		this->balas = recarga ? balas + 15: 40;
		break;
	default:
		this->balas = 1000;
		break;
	}
}

Tipo Arma::getTipo() {
	return tipo;
}

int Arma::getBalas() {
	return balas;
}

Proyectil* Arma::disparar() {
	Proyectil* proyectil = nullptr;

	if (balas > 0) {
		switch (this->tipo) {
		case Tipo::GunH:
			balas--;
			proyectil = new GunH(++contador);
			break;
		case Tipo::GunS:
			balas--;
			proyectil =  new GunS(++contador);
			break;
		case Tipo::GunR:
			balas--;
			proyectil =  new GunR(++contador);
			break;
		case Tipo::GunC:
			balas--;
			proyectil =  new GunC(++contador);
			break;
		case Tipo::Misil:
			proyectil = new Misil(++contador);
			break;
		case Tipo::Bomba:
			proyectil = new Bomba(++contador);
			break;
		case Tipo::Torreta:
			proyectil = new Torreta(++contador);
			break;
		case Tipo::Laser:
			proyectil = new Laser(++contador);
			break;
		default:
			break;
		}
	}

	return proyectil;
}
