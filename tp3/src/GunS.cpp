#include "GunS.h"

GunS::GunS(int id) : Proyectil(id) {
	this->danio = 50;
	this->cantidad = 30;
	this->cantidadRepuesto = 10;
	this->puntosOtorgados = 35;
	this->tipo = Tipo::GunS;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 10;			//ver si este tamanio esta bien 
	this->tamanio.y = 4;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunS::crearProyectil() {
	return new GunS(++contador);
}