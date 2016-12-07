/*
 * PickUpKillAll.cpp
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#include "PickUpKillAll.h"
#include "Juego.h"
#include "Jugador.h"
#include "Enemigo.h"

PickUpKillAll::PickUpKillAll(int id, Punto pos) : PickUp(id, pos) {
	this->tipo = Tipo::BonusKill;
}

void PickUpKillAll::aplicar(Juego* juego, Jugador* jugador) {
	auto it = juego->getObjetos().begin();
	while (it != juego->getObjetos().end()) {
		Objeto* obj = *it;
		if (obj->getTipo() == Tipo::Enemigo) {
			((Enemigo*)obj)->recibirDanio(10000);
		}
		it++;
	}
}
