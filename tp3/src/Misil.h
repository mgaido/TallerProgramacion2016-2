/*
 * Misil.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef MISIL_H_
#define MISIL_H_

#include "Proyectil.h"

class Misil: public Proyectil {
public:
	Misil(int id);
	virtual bool tieneCambios(Juego* juego);

};

#endif /* MISIL_H_ */
