/*
 * Misil.cpp
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#include "Misil.h"

Misil::Misil(int id) : Proyectil(id) {
	this->danio = 50;
	this->tipo = Tipo::Misil;
	this->tamanio.x = 40;
	this->tamanio.y = 40;
}

bool Misil::tieneCambios(Juego* juego) {
	velocidadProyectilX = 0.0003 + 0.0001 * (rand() % 4 - 2);
	velocidadProyectilY = - 0.0003;
	velocidadProyectilX *= (orientacion ? -1 : 1);

	return Proyectil::tieneCambios(juego);
}


