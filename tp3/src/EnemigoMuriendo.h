/*
 * EnemigoMuriendo.h
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#ifndef ENEMIGOMURIENDO_H_
#define ENEMIGOMURIENDO_H_

#include "Efecto.h"

class EnemigoMuriendo: public Efecto {
public:
	EnemigoMuriendo(int id, Punto pos, Punto tamanio);
};

#endif /* ENEMIGOMURIENDO_H_ */
