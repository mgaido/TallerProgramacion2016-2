#include "GunS.h"

GunS::GunS(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 50;
	this->cantidad = 30;
	this->cantidadRepuesto = 10;
	this->puntosOtorgados = 35;
	this->tipo = Tipo::GunS;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunS::crearProyectil() {
	return new GunS(++contador, idTirador);
}