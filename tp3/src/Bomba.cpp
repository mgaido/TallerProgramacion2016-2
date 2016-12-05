#include "Bomba.h"

Bomba::Bomba(int id, int idJugador) : Proyectil(id, idJugador) {
	this->tipo = Tipo::Bomba;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil * Bomba::crearProyectil(){
	return new Bomba(++contador, idTirador);
}
