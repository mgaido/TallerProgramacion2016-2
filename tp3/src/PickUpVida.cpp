/*
 * PickUpVida.cpp
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#include "PickUpVida.h"

#include "Juego.h"
#include "Jugador.h"

PickUpVida::PickUpVida(int id, Punto pos) : PickUp(id, pos) {
	this->tipo = Tipo::BonusVida;
}

void PickUpVida::aplicar(Juego* juego, Jugador* jugador) {
	jugador->restablecerEnergia();
}
