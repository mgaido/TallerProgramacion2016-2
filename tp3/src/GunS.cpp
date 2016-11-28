#include "GunS.h"

GunS::GunS(int id) : Proyectil(id) {
	this->danio = 50;
	this->cantidad = 30;
	this->cantidadRepuesto = 10;
	this->puntosOtorgados = 35;
	this->tipo = Tipo::GunS;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil* GunS::crearProyectil() {
	return new GunS(++contador);
}