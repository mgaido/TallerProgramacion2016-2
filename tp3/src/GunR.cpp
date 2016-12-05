#include "GunR.h"

GunR::GunR(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 80;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
	this->tipo = Tipo::GunR;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunR::crearProyectil() {
	return new GunR(++contador, idTirador);
}