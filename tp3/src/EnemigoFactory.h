/*
 * EnemigoFactory.h
 *
 *  Created on: Dec 6, 2016
 *      Author: rodrigo
 */

#ifndef ENEMIGOFACTORY_H_
#define ENEMIGOFACTORY_H_

#include "Utils.h"

class Juego;
class Enemigo;

class EnemigoFactory {
public:
	EnemigoFactory(int maxEnemigosSimultaneos, int maxEnemigos, std::function<Enemigo* (Juego*)> factory);
	bool spawnEnemigo(Juego* juego);
	void decrementarCantidadEnemigos();

private:
	std::function<Enemigo* (Juego*)> factory;
	int maxEnemigosSimultaneos;
	int maxEnemigos;
	int cantidadEnemigos;
	int totalSpawneados;
	micros tiempoUltimoEnemigo;
};

#endif /* ENEMIGOFACTORY_H_ */
