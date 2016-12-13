#include "Enemigo.h"

#include "PickUpArma.h"
#include "PickUpVida.h"
#include "PickUpKillAll.h"
#include "Juego.h"

Enemigo::Enemigo(int id, Config& _configuracion) : Soldado(id, _configuracion) {
	energia = 150;
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

	if (valor <= 60) {
		if (valor < 8)
			nuevoPickup = new PickUpKillAll(contador++, getPos());
		else if (valor < 20)
			nuevoPickup = new PickUpVida(contador++, getPos());
		else if (valor < 32)
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunH);
		else if (valor < 44)
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunS);
		else if (valor < 52)
			nuevoPickup = new PickUpArma(contador++, getPos(), Tipo::GunR);
		else
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

	if (tiempoCaminando > 0) {
		Escenario escenario = juego->getEscenario();

		if (velocCaminar < 0 && pos.x < escenario.getOffsetVista()) {
			pos.x = escenario.getOffsetVista();
			velocCaminar *= -1;
		}

		if (velocCaminar > 0 && pos.x + tamanio.x > (escenario.getOffsetVista() + escenario.getAnchoVista())) {
			pos.x = escenario.getOffsetVista() + escenario.getAnchoVista() - tamanio.x;
			velocCaminar *= -1;
		}
	}

	cambios |= comportamiento(juego);
	return cambios;
}


bool Enemigo::comportamiento(Juego* juego) {
	if (rand() % 17 != 0)
		return false;

	if ( tiempoCaminando > 0) {
		int millisCreacion = (tiempo() - tiempoCreacion) / 1000;
		int millis = 1000 * (rand() % 5 + 1);

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
		Punto posJugador = posJugadorMasCercano(juego);

		if (orientacion && posJugador.x > pos.x)
			orientacion = false;
		else if (! orientacion && posJugador.x < pos.x)
			orientacion = true;

		int millisDisparo = (tiempo() - tiempoUltimoDisparo) / 1000;
		int millis = 1000 * (rand() % 4) + 1;
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

Punto Enemigo::posJugadorMasCercano(Juego* juego) {

	auto it = juego->getObjetos().begin();

	Punto posCercana;
	posCercana.x = pos.x + 10000 * (orientacion ? -1 : 1);
	posCercana.y = pos.y;
	double dist = distancia(pos, posCercana);

	while (it != juego->getObjetos().end()) {
		Objeto* obj = *it;
		if (obj->getTipo() == Tipo::Jugador) {
			double dist2 = distancia(pos, obj->getPos());
			if (dist2 < dist) {
				dist = dist2;
				posCercana = obj->getPos();
				posCercana.x += obj->getTamanio().x / 2;
				posCercana.y += obj->getTamanio().y / 2;
			}
		}
		it++;
	}

	return posCercana;
}
