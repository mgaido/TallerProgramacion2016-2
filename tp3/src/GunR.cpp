#include "GunR.h"

GunR::GunR(int id) : Proyectil(id) {
	this->danio = 80;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
	this->tipo = Tipo::GunR;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil* GunR::crearProyectil() {
	return new GunR(++contador);
}