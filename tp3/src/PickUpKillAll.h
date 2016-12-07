/*
 * PickUpKillAll.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef PICKUPKILLALL_H_
#define PICKUPKILLALL_H_

#include "PickUp.h"

class PickUpKillAll: public PickUp {
public:
	PickUpKillAll(int id, Punto pos);
	virtual void aplicar(Juego* juego, Jugador* jugador);
};

#endif /* PICKUPKILLALL_H_ */
