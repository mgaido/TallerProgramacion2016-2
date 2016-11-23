#include "GunH.h"

GunH::GunH(int id) : Proyectil(id) {
	this->danio = 20;
	this->cantidad = 200;
	this->cantidadRepuesto = 150;
	this->puntosOtorgados = 15;
}