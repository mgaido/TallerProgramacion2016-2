/*
 * Juego.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Juego.h"

Juego::Juego() {
	iniciado = false;
	contador = 0;
}

bool Juego::estaIniciado() {
	return iniciado;
}

Jugador* Juego::nuevoJugador(std::string nombre) {
	iniciado = true;

	Jugador* jugador = new Jugador(++contador, nombre);
	Actualizacion actualizacion;
	actualizacion.setId(jugador->getId());
	actualizacion.setEvento(Evento::Agregar);
	actualizacion.setTipo(jugador->getTipo());
	actualizacion.setEstado(jugador->getEstado());
	actualizacion.setTamanio(jugador->getTamanio());
	lock.lock();
	objetos.push_back(jugador);
	actualizaciones.push_back(actualizacion);
	lock.unlock();

	return jugador;
}

bool Juego::getActualizaciones(Bytes& bytes) {
	bool hayActualizaciones = actualizaciones.size() > 0;

	lock.lock();
	auto it = objetos.begin();
	while (it != objetos.end()) {
		Objeto* obj = *it;
		if (obj->tieneCambios()) {
			hayActualizaciones = true;
			Actualizacion upd;
			upd.setId(obj->getId());
			upd.setEvento(Evento::Modificar);
			upd.setTipo(obj->getTipo());
			upd.setEstado(obj->getEstado());
			upd.setPos(obj->getPos());
			upd.setTamanio(obj->getTamanio());
			actualizaciones.push_back(upd);
		}
		it++;
	}
	lock.unlock();
	if (hayActualizaciones) {
		bytes.put(UPD);
		bytes.putAll(actualizaciones);
	}
	actualizaciones.clear();
	return hayActualizaciones;
}

void Juego::getEstado(std::vector<Actualizacion>& estado) {
	lock.lock();
	auto it = objetos.begin();
	while (it != objetos.end()) {
		Objeto* obj = *it;
		Actualizacion upd;
		upd.setId(obj->getId());
		upd.setEvento(Evento::Agregar);
		upd.setTipo(obj->getTipo());
		upd.setEstado(obj->getEstado());
		upd.setPos(obj->getPos());
		upd.setTamanio(obj->getTamanio());
		estado.push_back(upd);
		it++;
	}
	lock.unlock();
}
