/*
 * Puntaje.h
 *
 *  Created on: 13 dic. 2016
 *      Author: Rod
 */

#ifndef PUNTAJE_H_
#define PUNTAJE_H_

#include "Utils.h"

class Puntaje {
public:
	Puntaje();

	void sumarPuntos(int puntos);

	void nuevoNivel();
	void nivelGanado();

	int getPuntosJuego() const;
	int getPuntosNivel() const;

private:
	int puntosNivel;
	int puntosJuego;
};

#endif /* PUNTAJE_H_ */
