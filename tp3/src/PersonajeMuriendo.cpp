/*
 * PersonajeMuriendo.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "PersonajeMuriendo.h"

#include "Personaje.h"

PersonajeMuriendo::PersonajeMuriendo(int id, Personaje* personaje) : Efecto(id, personaje->getPos(), personaje->getTamanio(), 0) {
	this->duracion = 900;
	this->tipoObjeto = personaje->getTipo();
}

