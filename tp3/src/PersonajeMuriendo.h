/*
 * PersonajeMuriendo.h
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#ifndef PERSONAJEMURIENDO_H_
#define PERSONAJEMURIENDO_H_

#include "Efecto.h"

class Enemigo;

class PersonajeMuriendo: public Efecto {
public:
	PersonajeMuriendo(int id, Personaje* personaje);
};

#endif /* PERSONAJEMURIENDO_H_ */
