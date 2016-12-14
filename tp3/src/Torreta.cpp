/*
 * Canion.cpp
 *
 *  Created on: 7 dic. 2016
 *      Author: Rod
 */

#include "Torreta.h"

Torreta::Torreta(int id) : Proyectil(id) {
	this->danio = 40;
	this->tipo = Tipo::Torreta;
	this->estado = Estado::Normal;
	this->tamanio.x = 50;			//ver si este tamanio esta bien
	this->tamanio.y = 50;
}

