#include "Bomba.h"
#include "Juego.h"

Bomba::Bomba(int id) : Proyectil(id) {
	this->tipo = Tipo::Bomba;
	this->estado = Estado::ProyectilEnMovimiento;
	this->velocidadProyectilX = 0;
	this->velocidadProyectilY = 0;
	this->tamanio.x = 32;
	this->tamanio.y = 50;
	this->danio = 100;
}

bool Bomba::tieneCambios(Juego* juego) {
	micros  t = tiempo() - tiempoEnMovimiento;
	velocidadProyectilY -= t*juego->getConfiguracion().getGravedad();
	return Proyectil::tieneCambios(juego);
}

void Bomba::setOrientacionX(bool orientacion) {
	orientacion = false;
	this->velocidadProyectilX = 0;
}
