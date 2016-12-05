/*
 * Teclas.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Logger.h"
#include "Teclas.h"

Teclas::Teclas() {
	estado = 0;
}

bool Teclas::arriba() {
	return (estado & (1 << ARRIBA)) != 0;
}

bool Teclas::saltar() {
	return (estado & (1 << SPACE)) != 0;
}

bool Teclas::abajo() {
	return (estado & (1 << ABAJO)) != 0;
}

bool Teclas::izq() {
	return (estado & (1 << IZQ)) != 0;
}

bool Teclas::der() {
	return (estado & (1 << DER)) != 0;
}

bool Teclas::disparar() {
	return (estado & (1 << Z)) != 0;
}

bool Teclas::recargar() {
	return (estado & (1 << R)) != 0;
}

int Teclas::getEstado() {
	return estado;
}

void Teclas::setEstado(int estado) {
	this->estado = estado;
}

bool Teclas::evento(char tecla, bool presionada) {
	int _estado = estado;
	if (presionada)
		estado |= 1 << tecla;
	else
		estado &= ~(1 << tecla);
	return estado != _estado;
}
