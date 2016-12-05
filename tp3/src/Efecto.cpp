/*
 * Efecto.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "Efecto.h"

Efecto::Efecto(int id, Punto pos, Punto tamanio, int puntos) : Objeto(id) {
	this->pos = pos;
	this->tamanio = tamanio;
	this->estado = Estado::Quieto;
	this->frame = 0;
	this->puntos = puntos;
	this->inicio = tiempo();
}

Efecto::~Efecto() {
}

bool Efecto::esVisible() {
	return (tiempo() - inicio)/1000 < duracion;
}

bool Efecto::tieneCambios(std::vector<Plataforma*>& plataformas) {
	return true;
}

std::string Efecto::getNombre() {
	if (puntos > 0)
		return std::to_string(puntos);
	else
		return "";
}




