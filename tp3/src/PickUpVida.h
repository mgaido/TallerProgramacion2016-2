/*
 * PickUpVida.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef PICKUPVIDA_H_
#define PICKUPVIDA_H_

#include "PickUp.h"

class PickUpVida : public PickUp {
public:
	PickUpVida(int id, Punto pos);
	virtual void aplicar(Juego* juego, Jugador* jugador);

};

#endif /* PICKUPVIDA_H_ */
