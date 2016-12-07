/*
 * Objeto.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: Rod
 */

#include "Objeto.h"
#include "Efecto.h"
#include "Juego.h"


Objeto::Objeto(int id) {
	this->id = id;
	this->frame = 0;
	this->orientacion = false;
	this->visible = true;
	this->tipo = Tipo::Void;
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

bool Objeto::tieneCambios(Juego* juego) {
	return false;
}

int Objeto::getFrame() {
	return frame++;
}

bool Objeto::getOrientacion() {
	return orientacion;
}

void Objeto::setTipo(Tipo tipo) {
	this->tipo = tipo;
}

bool Objeto::esVisible() {
	return visible;
}

bool Objeto::colisionaCon(Objeto* otro) {

	if (((getPos().x + getTamanio().x) < (otro->getPos().x))
			|| (getPos().x > otro->getPos().x + otro->getTamanio().x)) {
		return false;
	} else if (((getPos().y + getTamanio().y) < (otro->getPos().y))
			|| (getPos().y > otro->getPos().y + otro->getTamanio().y)) {
		return false;
	} else {
		return true;
	}
}

Efecto* Objeto::efectoAlDesaparecer() {
	return nullptr;
}

std::string Objeto::getNombre() {
	return std::string();
}

EstadoObj Objeto::getEstadoObj(Escenario& escenario) {
	EstadoObj estadoObj;
	estadoObj.setId(getId());
	estadoObj.setTipo(getTipo());
	estadoObj.setEstado(getEstado());
	estadoObj.setTipo(getTipo());
	Punto pos;
	pos.x = getPos().x - escenario.getOffsetVista();
	pos.y = escenario.getNivelPiso() - getTamanio().y - getPos().y;
	estadoObj.setPos(pos);

	estadoObj.setTamanio(getTamanio());
	estadoObj.setFrame(getFrame());
	estadoObj.setOrientacion(getOrientacion());
	estadoObj.setNombre(getNombre());

	return estadoObj;
}
