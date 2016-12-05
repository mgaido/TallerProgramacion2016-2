#include "GunF.h"

GunF::GunF(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 35;
	this->cantidad = 30;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 40;
	this->tipo = Tipo::GunF;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->frame = 0;
}

Proyectil* GunF::crearProyectil() {
	return new GunF(++contador, idTirador);
}