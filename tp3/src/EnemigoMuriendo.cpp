/*
 * EnemigoMuriendo.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "EnemigoMuriendo.h"

EnemigoMuriendo::EnemigoMuriendo(int id, Punto pos, Punto tamanio) : Efecto(id, pos, tamanio, 0) {
	this->duracion = 900;
	this->tipo = Tipo::EnemigoMuriendo;
}

