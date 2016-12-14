/*
 * ImpactoBala.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "ImpactoBala.h"

ImpactoBala::ImpactoBala(int id, Proyectil* proyectil) : Efecto(id, proyectil->getPos(), proyectil->getTamanio(), proyectil->getPuntos()) {
	this->duracion = 450;
	this->tipoObjeto = proyectil->getTipo();

	switch (tipoObjeto) {
	case Tipo::GunS:
		tamanio.x *=2;
		tamanio.y *=2;
		break;
	case Tipo::GunR:
		tamanio.x *=3;
		tamanio.y *=3;
		break;
	case Tipo::GunC:
		tamanio.x *=4;
		tamanio.y *=4;
		break;
	case Tipo::Misil:
		tamanio.x *=4;
		tamanio.y *=4;
		break;
	case Tipo::Bomba:
		tamanio.x *=5;
		tamanio.y *=5;
		break;
	case Tipo::Torreta:
		tamanio.x *=2;
		tamanio.y *=2;
		break;
	case Tipo::Laser:
		tamanio.x *=3;
		tamanio.y *=3;
		break;
	default:
		break;
	}
}
