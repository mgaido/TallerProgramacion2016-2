#include "GunC.h"

GunC::GunC(int id) : Proyectil(id) {
	this->danio = 150;
	this->cantidad = 40;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 100;
}