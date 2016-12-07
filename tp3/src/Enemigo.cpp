#include "Enemigo.h"

#include "PickUpArma.h"
#include "PickUpVida.h"
#include "PickUpKillAll.h"
#include "Juego.h"

Enemigo::Enemigo(int id, Config& _configuracion) : Soldado(id, _configuracion) {
	energia = 250;
	tipo = Tipo::Enemigo;
	tiempoCreacion = tiempo();
	tiempoUltimoDisparo = tiempo();
	arma.setTipo(Tipo::GunH);
	caminar(Direccion::IZQUIERDA);
}

Enemigo::Enemigo(int id, Config& _configuracion, Escenario& escenario) : Enemigo(id, _configuracion) {
	pos.x = escenario.getOffsetVista() + escenario.getAnchoVista();
}

Enemigo::~Enemigo() {
}

PickUp* Enemigo::spawnPickUp() {

	PickUp* nuevoPickup = nullptr;

	int valor = rand() % 100;

	if (valor < 30) { //30% dropear bonus al morir
		if (valor < 5) //16%
			nuevoPickup = new PickUpKillAll(contador++, getPos());
		else if (valor < 15) //33% vida
			nuevoPickup = new PickUpVida(contador++, getPos());
		else if (valor < 21) //20% H
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunH);
		else if (valor < 25)  // 13% S
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunS);
		else if (valor < 28)  // 10% R
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunR);
		else // 6% C
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunC);
	}
	return nuevoPickup;
}


void Enemigo::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);

	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -configuracion.getVelocidadX() /2;
	else
		velocCaminar = configuracion.getVelocidadX() /2;

	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}

void Enemigo::saltar() {
	std::unique_lock<std::mutex> lock(mutex);

	if (tiempoSalto == 0) {
		velocSaltoY = 0.0009;
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
	cambios = true;
}

bool Enemigo::tieneCambios(Juego* juego) {
	bool cambios = Soldado::tieneCambios(juego);
	cambios |= comportamiento(juego);
	return cambios;
}


bool Enemigo::comportamiento(Juego* juego) {
	if (rand() % 23 != 0)
		return false;

	if ( tiempoCaminando > 0) {
		int millisCreacion = (tiempo() - tiempoCreacion) / 1000;
		int millis = 1000 * (rand() % 8 + 2);

		if (millisCreacion > millis) {
			this->detenerse();
			return true;
		}

		millis = 1000 * (rand() % 3 + 1);
		if (millisCreacion > millis) {
			this->saltar();
			return true;
		}
	} else {
		int millisDisparo = (tiempo() - tiempoUltimoDisparo) / 1000;
		int millis = 1000 * (rand() % 5) + 1;
		if (millisDisparo > millis) {
			tiempoUltimoDisparo = tiempo();
			return this->disparar(juego);
		}
	}

	return false;
}

void Enemigo::setTiempoCreacion(micros tiempoCreacion){
	this->tiempoCreacion = tiempoCreacion;
}

