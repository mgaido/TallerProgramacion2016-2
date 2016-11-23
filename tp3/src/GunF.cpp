#include "GunF.h"

GunF::GunF(int id) : Proyectil(id) {
	this->danio = 35;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
}