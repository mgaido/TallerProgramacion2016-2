#include "GunH.h"

GunH::GunH(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 20;
	this->cantidad = 200;
	this->cantidadRepuesto = 150;
	this->puntosOtorgados = 15;
	this->tipo = Tipo::GunH;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 50;			//ver si este tamanio esta bien
	this->tamanio.y = 50;
	this->orientacion = false;
	this->frame = 0;
	this->velocidadProyectilY = 0;
}

Proyectil* GunH::crearProyectil() {
	return new GunH(++contador, idTirador);
}
