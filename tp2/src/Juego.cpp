/*
 * Juego.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Juego.h"

Juego::Juego(Config& _configuracion) :  configuracion(_configuracion) {
	iniciado = false;
	cambios = true;
	contador = 0;
	escenario = Escenario(configuracion.getLongitud(), configuracion.getTamanioVentana().x, configuracion.getNivelPiso());
}

bool Juego::estaIniciado() {
	return iniciado;
}

Jugador* Juego::nuevoJugador(std::string nombre) {
	iniciado = true;

	Jugador* jugador = new Jugador(++contador, nombre);
	jugador->getTamanio().x = configuracion.getTamanioJugador().x;
	jugador->getTamanio().y = configuracion.getTamanioJugador().y;
	jugador->getPos().x = escenario.getOffsetVista();
	lock.lock();
	jugadores.push_back(jugador);
	cambios = true;
	lock.unlock();

	return jugador;
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estado;

	lock.lock();

	int loop = escenario.getLongitud();
	if (escenario.getOffsetVista() <= loop)
		loop = 0;
	escenario.setOffsetVista(escenario.getOffsetVista() - loop);

	//Actualizar posiciones de jugadores y encontrar el minimo en X
	int minX = escenario.getLongitud()*2;
	auto it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;
		cambios |= obj->tieneCambios() || loop > 0;
		obj->getPos().x -= loop;

		if (obj->getPos().x < minX && obj->getEstado() != Estado::Desconectado)
			minX = obj->getPos().x + obj->getTamanio().x / 2;
		it++;
	}

	//Actualizar el offset si es necesario
	if (minX > escenario.getOffsetVista() + escenario.getAnchoVista()/2) {
		cambios = true;
		int offset = std::min<int>(minX - escenario.getAnchoVista()/2, escenario.getOffsetVista() + 10);
		escenario.setOffsetVista(offset);
	}

	//Actualizar posiciones de jugadores para que no se salgan de pantalla
	it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;

		if ((obj->getPos().x + obj->getTamanio().x) > (escenario.getOffsetVista() + escenario.getAnchoVista())) {
			cambios = true;
			obj->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista()- obj->getTamanio().x;
		}

		if (obj->getPos().x < escenario.getOffsetVista()) {
			cambios = true;
			obj->getPos().x = escenario.getOffsetVista();
		}
		it++;
	}

	//Generar estado para la vista
	if (cambios) {
		it = jugadores.begin();
		while (it != jugadores.end()) {
			Objeto* obj = *it;

			EstadoObj estadoObj;
			estadoObj.setId(obj->getId());
			estadoObj.setTipo(obj->getTipo());
			estadoObj.setEstado(obj->getEstado());
			Punto pos;
			pos.x = obj->getPos().x - escenario.getOffsetVista();
			pos.y = escenario.getNivelPiso() - obj->getTamanio().y - obj->getPos().y;
			estadoObj.setPos(pos);
			estadoObj.setTamanio(obj->getTamanio());

			if (obj->getTipo() == Tipo::Jugador)
				estadoObj.setNombre(((Jugador*)obj)->getNombre());

			estado.push_back(estadoObj);
			it++;
		}
	}

	rv = cambios;
	cambios = false;
	lock.unlock();

	if (rv) {
		bytes.put(escenario.getOffsetVista());
		bytes.putAll(estado);
	}
	return rv;
}
