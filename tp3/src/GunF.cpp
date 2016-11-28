#include "GunF.h"

GunF::GunF(int id) : Proyectil(id) {
	this->danio = 35;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
	this->tipo = Tipo::GunF;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil* GunF::crearProyectil() {
	return new GunF(++contador);
}