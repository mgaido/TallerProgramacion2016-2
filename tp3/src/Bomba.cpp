#include "Bomba.h"

Bomba::Bomba(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 300;
	this->cantidad = 1000000;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 100;
	this->tamanio.x = 15;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->frame = 0;
	this->tipo = Tipo::Bomba;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil * Bomba::crearProyectil(){
	return new Bomba(++contador, idTirador);
}
