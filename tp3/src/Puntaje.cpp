/*
 * Puntaje.cpp
 *
 *  Created on: 13 dic. 2016
 *      Author: Rod
 */

#include "Puntaje.h"

Puntaje::Puntaje() {
	puntosNivel = 0;
	puntosJuego = 0;
}

void Puntaje::sumarPuntos(int puntos) {
	this->puntosNivel += puntos;
}

void Puntaje::nivelGanado() {
	puntosJuego += puntosNivel;
}

void Puntaje::nuevoNivel() {
	puntosNivel = 0;
}

int Puntaje::getPuntosJuego() const {
	return puntosJuego;
}

int Puntaje::getPuntosNivel() const {
	return puntosNivel;
}
