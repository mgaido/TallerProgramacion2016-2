/*
 * EnemigoMuriendo.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "EnemigoMuriendo.h"

EnemigoMuriendo::EnemigoMuriendo(int id, Punto pos, Punto tamanio, Tipo tipoProyectil) : Efecto(id, pos, tamanio, 0) {
	this->duracion = 1000;

	switch (tipoProyectil) {
	case Tipo::GunH:
		this->tipo = Tipo::EnemigoMuriendo;
		break;
	case Tipo::GunC:
		this->tipo = Tipo::EnemigoMuriendoExp;
		break;
	case Tipo::GunS:
		this->tipo = Tipo::EnemigoMuriendo;
		break;
	case Tipo::GunR:
		this->tipo = Tipo::EnemigoMuriendoExp;
		break;
	case Tipo::GunF:
		this->tipo = Tipo::EnemigoMuriendoFuego;
		break;
	}

}

