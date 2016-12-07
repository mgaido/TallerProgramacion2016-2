/*
 * EnemigoFactory.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: rodrigo
 */

#include "EnemigoFactory.h"
#include "Enemigo.h"
#include "Juego.h"

EnemigoFactory::EnemigoFactory(int maxEnemigosSimultaneos, int maxEnemigos, std::function<Enemigo* (Juego*)> factory) {
	this->cantidadEnemigos = 0;
	this->totalSpawneados = 0;
	this->maxEnemigosSimultaneos = maxEnemigosSimultaneos;
	this->maxEnemigos = maxEnemigos;
	this->factory = factory;
	this->tiempoUltimoEnemigo = tiempo();
}

bool EnemigoFactory::spawnEnemigo(Juego* juego) {
	if (cantidadEnemigos >= maxEnemigosSimultaneos)
		return false;

	if (maxEnemigos > 0 && totalSpawneados >= maxEnemigos)
		return false;

	int millisUltimo = (tiempo() - tiempoUltimoEnemigo) / 1000;
	int millis = ((rand() % 6) + 2) * 1000;

	if (millisUltimo < millis)
		return false;

	Escenario escenario = juego->getEscenario();

	Enemigo* nuevoEnemigo = factory(juego);
	cantidadEnemigos++;
	totalSpawneados++;
	tiempoUltimoEnemigo = tiempo();

	juego->agregarObjeto(nuevoEnemigo);

	return true;
}

void EnemigoFactory::decrementarCantidadEnemigos() {
	this->cantidadEnemigos--;
}
