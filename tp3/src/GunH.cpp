#include "GunH.h"

GunH::GunH(int id) : Proyectil(id) {
	this->danio = 20;
	this->cantidad = 200;
	this->cantidadRepuesto = 150;
	this->puntosOtorgados = 15;
	this->tipo = Tipo::GunH;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 10;			//ver si este tamanio esta bien 
	this->tamanio.y = 4;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunH::crearProyectil() {
	return new GunH(++contador);
}