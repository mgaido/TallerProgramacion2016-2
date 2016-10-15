/*
 * Juego.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Juego.h"

Juego::Juego() {
	iniciado = false;
	cambios = true;
	contador = 0;
}

bool Juego::estaIniciado() {
	return iniciado;
}

Jugador* Juego::nuevoJugador(std::string nombre) {
	iniciado = true;

	Jugador* jugador = new Jugador(++contador, nombre);
	lock.lock();
	objetos.push_back(jugador);
	cambios = true;
	lock.unlock();

	return jugador;
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estado;

	lock.lock();
	auto it = objetos.begin();
	while (it != objetos.end()) {
		Objeto* obj = *it;
		cambios |= obj->tieneCambios();

		EstadoObj upd;
		upd.setId(obj->getId());
		upd.setTipo(obj->getTipo());
		upd.setEstado(obj->getEstado());
		upd.setPos(obj->getPos());
		upd.setTamanio(obj->getTamanio());
		estado.push_back(upd);
		it++;
	}
	rv = cambios;
	cambios = false;
	lock.unlock();

	if (rv)
		bytes.putAll(estado);
	return rv;
}
