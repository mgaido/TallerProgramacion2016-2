/*
 * ImpactoBala.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "ImpactoBala.h"

ImpactoBala::ImpactoBala(int id, Proyectil* proyectil) : Efecto(id, proyectil->getPos(), proyectil->getTamanio(), proyectil->getPuntos()) {
	this->duracion = 100;
	this->tipoObjeto = proyectil->getTipo();
}
