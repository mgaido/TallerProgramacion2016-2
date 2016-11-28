#include "Proyectil.h"

Proyectil::Proyectil(int id) : Objeto(id) {
	tiempoUltimoDisparo = 0;
	cambios = false;
	velocidadProyectil = 200;
	tiempoEnMovimiento = 0;
}

void Proyectil::recargar() {
	cantidad = cantidad + cantidadRepuesto;
}

int Proyectil::getPuntos() {
	return puntosOtorgados;
}

Proyectil* Proyectil::disparar() {
	Proyectil* nuevoProyectil = NULL;
	if (cantidad > 0) {
		cantidad = cantidad - 1;
		nuevoProyectil = crearProyectil();
	}
	return nuevoProyectil;
}

Proyectil::~Proyectil() {}

void Proyectil::setOrientacion(bool nuevaOrientacion) {
	orientacion = nuevaOrientacion;
	tiempoEnMovimiento = tiempo();
	if (orientacion)
		velocidadProyectil = -0.0004;
	else
		velocidadProyectil = 0.0004;
}

int Proyectil::getDanio(){
	return danio;
}

void Proyectil::trayectoria() {
	micros t = 0;
	double vx = 0;

	if (tiempoEnMovimiento > 0) {
		vx = velocidadProyectil;
		t = tiempo() - tiempoEnMovimiento;
		tiempoEnMovimiento += t;
	}

	cambios |= t != 0;

	if (vx != 0)
		orientacion = vx < 0;

	pos.x += (int)round(vx*t);
	

}

Proyectil* Proyectil::crearProyectil(){
	return NULL;
}

bool Proyectil::tieneCambios() {
	std::unique_lock<std::mutex> lock(mutex);
	trayectoria();
	return cambios;
}

void Proyectil::setPos(Punto nuevaPosicion) {
	this->pos.x = nuevaPosicion.x;
	this->pos.y = nuevaPosicion.y + 140;
}
