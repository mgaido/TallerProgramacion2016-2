/*
 * Escenario.cpp
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#include "Escenario.h"

Escenario::Escenario(){
	this->longitud = 0;
	this->anchoVista = 0;
	this->nivelPiso = 0;
	this->offsetVista = 0;
}

Escenario::Escenario(int longitud, int anchoVista, int altoVista, int nivelPiso) {
	this->longitud = longitud;
	this->anchoVista = anchoVista;
	this->altoVista = altoVista;
	this->nivelPiso = nivelPiso;
	this->offsetVista = 0;
}

int Escenario::getAnchoVista() const {
	return anchoVista;
}

int Escenario::getAltoVista() const {
	return this->altoVista;
}

int Escenario::getLongitud() const {
	return longitud;
}

int Escenario::getNivelPiso() const {
	return nivelPiso;
}

int Escenario::getOffsetVista() const {
	return offsetVista;
}

void Escenario::setOffsetVista(int offsetVista) {
	this->offsetVista = offsetVista;
}
