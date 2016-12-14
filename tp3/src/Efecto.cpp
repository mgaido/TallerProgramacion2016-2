/*
 * Efecto.cpp
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#include "Efecto.h"

#include "Enums.h"
#include "Estado.h"
#include "Utils.h"

Efecto::Efecto(int id, Punto pos, Punto tamanio, int puntos) : Objeto(id) {
	this->pos = pos;
	this->tamanio = tamanio;
	this->tipo = Tipo::Efecto;
	this->estado = Estado::Efecto;
	this->frame = 0;
	this->puntos = puntos;
	this->inicio = tiempo();
	this->duracion = 0;
	tipoObjeto = Tipo::Void;
}

Efecto::~Efecto() {
}

bool Efecto::tieneCambios(Juego* juego) {
	visible = (tiempo() - inicio)/1000 < duracion;
	return true;
}

EstadoObj Efecto::getEstadoObj(Escenario& escenario) {
	EstadoObj estadoObj = Objeto::getEstadoObj(escenario);
	estadoObj.setTipo(tipoObjeto);
	if (puntos > 0)
		estadoObj.setNombre(std::to_string(puntos));
	return estadoObj;
}


