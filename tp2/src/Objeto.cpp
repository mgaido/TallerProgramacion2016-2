/*
 * Objeto.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: Rod
 */

#include "Objeto.h"

Objeto::Objeto(int id) {
	this->id = id;
}

Objeto::~Objeto() {
}

int Objeto::getId() {
	return id;
}

Punto Objeto::getPos() {
	return pos;
}

Punto Objeto::getTamanio() {
	return tamanio;
}

Estado Objeto::getEstado() {
	return estado;
}

Tipo Objeto::getTipo() {
	return tipo;
}

bool Objeto::tieneCambios() {
	return false;
}

