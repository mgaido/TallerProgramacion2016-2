/*
 * Objeto.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: Rod
 */

#include "Objeto.h"

Escenario::Escenario(){}

Escenario::Escenario(int longitud, int anchoVista, int nivelPiso) {
	this->longitud = longitud;
	this->anchoVista = anchoVista;
	this->nivelPiso = nivelPiso;
	this->offsetVista = 0;
}

int Escenario::getAnchoVista() const {
	return anchoVista;
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

Objeto::Objeto(int id) {
	this->id = id;
	this->frame = 0;
	this->orientacion = false;
}

Objeto::~Objeto() {
}

int Objeto::getId() {
	return id;
}

Punto& Objeto::getPos() {
	return pos;
}

Punto& Objeto::getTamanio() {
	return tamanio;
}

Estado Objeto::getEstado() {
	return estado;
}

Tipo Objeto::getTipo() {
	return tipo;
}

bool Objeto::tieneCambios() {
	return false;
}

int Objeto::getFrame() {
	return frame++;
}

bool Objeto::getOrientacion() {
	return orientacion;
}

