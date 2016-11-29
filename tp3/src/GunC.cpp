#include "GunC.h"

GunC::GunC(int id) : Proyectil(id) {
	this->danio = 150;
	this->cantidad = 40;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 100;
	this->tipo = Tipo::GunC;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 10;			//ver si este tamanio esta bien 
	this->tamanio.y = 4;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunC::crearProyectil() {
	return new GunC(++contador);
}