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


void Juego::cargarDatosJugadorNuevo() {
	Actualizacion actualizacion;
	auto it = objetos.begin();
	while (it != objetos.end()) {
		if ((*it)->getTipo() == Tipo::Jugador) {
			Jugador* jugAux = (Jugador*)(*it);

			actualizacion.setId(jugAux->getId());
			actualizacion.setEvento(Evento::Agregar);
			actualizacion.setTipo(jugAux->getTipo());
			actualizacion.setEstado(jugAux->getEstado());
			actualizacion.setTamanio(jugAux->getTamanio());
			actualizacion.setPos(jugAux->getPos());
			lock.lock();
			actualizaciones.push_back(actualizacion);
			lock.unlock();
		}
		it++;
	}
}

Jugador* Juego::nuevoJugador(std::string ip, std::string nombre) {
	iniciado = true;
	bool existe = false;
	Jugador* jugador = NULL;
	Actualizacion actualizacion;

	auto it = objetos.begin();
	while (it != objetos.end() && !existe) {
		if ((*it)->getTipo() == Tipo::Jugador) {
			Jugador* jugAux = (Jugador*)(*it);
			//std::cout << jugAux->getNombre() << std::endl;
			if (nombre == jugAux->getNombre()) {
				existe = true;
				jugador = jugAux;
			}
		}
		it++;
	}
	if (!existe) {
		jugador = new Jugador(++contador, nombre);
		actualizacion.setId(jugador->getId());
		actualizacion.setEvento(Evento::Agregar);
		actualizacion.setTipo(jugador->getTipo());
		actualizacion.setEstado(jugador->getEstado());
		actualizacion.setTamanio(jugador->getTamanio());
		lock.lock();
		objetos.push_back(jugador);
		actualizaciones.push_back(actualizacion);
		lock.unlock();
	}
	cargarDatosJugadorNuevo();
	return jugador;
}

bool Juego::getActualizaciones(Bytes& bytes) {
	bool hayActualizaciones = actualizaciones.size() > 0;

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
			actualizaciones.push_back(upd);
		}
		it++;
	}

	if (hayActualizaciones) {
		bytes.put(UPD);
		bytes.putAll(actualizaciones);
	}
	actualizaciones.clear();
	return hayActualizaciones;
}
