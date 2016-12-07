/*
 * Laser.cpp
 *
 *  Created on: 7 dic. 2016
 *      Author: Rod
 */

#include "Laser.h"

Laser::Laser(int id) : Proyectil(id){
	this->danio = 60;
	this->tipo = Tipo::Laser;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 30;			//ver si este tamanio esta bien
	this->tamanio.y = 15;
}

