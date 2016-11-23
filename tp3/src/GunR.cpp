#include "GunR.h"

GunR::GunR(int id) : Proyectil(id) {
	this->danio = 80;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
}