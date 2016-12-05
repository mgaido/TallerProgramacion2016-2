/*
 * ImpactoBala.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "ImpactoBala.h"

ImpactoBala::ImpactoBala(int id, Proyectil* proyectil) : Efecto(id, proyectil->getPos(), proyectil->getTamanio(), proyectil->getPuntos()) {
	this->duracion = 100;

	switch (proyectil->getTipo()) {
	case Tipo::GunH:
		this->tipo = Tipo::ImpactoH;
		break;
	case Tipo::GunC:
		this->tipo = Tipo::ImpactoExp;
		break;
	case Tipo::GunS:
		this->tipo = Tipo::ImpactoS;
		break;
	case Tipo::GunR:
		this->tipo = Tipo::ImpactoExp;
		break;
	case Tipo::GunF:
		this->tipo = Tipo::ImpactoF;
		break;
	default:
		this->tipo = Tipo::ImpactoH;
	}

}
