#include "Personaje.h"
#include "PersonajeMuriendo.h"
#include "GunH.h"
#include "Juego.h"


Personaje::Personaje(int id, Config & _configuracion) : Objeto(id), configuracion(_configuracion) {
	estado = Estado::Normal;
	energia = 0;
}

int Personaje::getEnergia() {
	return energia;
}

Arma& Personaje::getArma() {
	return arma;
}

bool Personaje::recibirDanio(int danio) {
	bool muerto = false;

	if (energia > danio) {
		energia -= danio;
	} else {
		energia = 0;
		muerto = true;
	}
	visible = ! muerto;
	return muerto;
}

Proyectil* Personaje::disparar(Juego* juego) {
	Proyectil* nuevoProyectil = arma.disparar();

	if (nuevoProyectil != nullptr) {
		nuevoProyectil->setTirador(this);
		Punto pos;
		pos.x = getPos().x + (getOrientacion() ? 0 : getTamanio().x);
		pos.y = getPos().y - getTamanio().y * 0.6;
		nuevoProyectil->setPos(pos);
		nuevoProyectil->setOrientacionX(orientacion);
		juego->agregarObjeto(nuevoProyectil);

		return nuevoProyectil;
	}
	return nullptr;
}


Efecto* Personaje::efectoAlDesaparecer() {
	return new PersonajeMuriendo(++contador, this);
}

