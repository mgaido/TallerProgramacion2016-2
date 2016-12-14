#include "GunH.h"

GunH::GunH(int id) : Proyectil(id) {
	this->danio = 20;
	this->puntosOtorgados = 15;
	this->tipo = Tipo::GunH;
	this->tamanio.x = 50;			//ver si este tamanio esta bien
	this->tamanio.y = 50;
	this->cooldown = 100;
}

