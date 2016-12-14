/*
 * Equipo.cpp
 *
 *  Created on: 13 dic. 2016
 *      Author: Rod
 */

#include "Equipo.h"


Equipo::Equipo() {
	cantJugadores = 0;
}

Equipo::~Equipo() {
}

bool Equipo::tieneJugador(std::string nombre) {
	std::array<char, 100> c = toCharArray<100>(nombre);
	for (int i = 0; i < cantJugadores; i++) {
		if (fromCharArray(c) == fromCharArray(nombres[i]))
			return true;
	}
	return false;
}

void Equipo::agregarJugador(std::string nombre) {
	std::array<char, 100> c = toCharArray<100>(nombre);
	nombres[cantJugadores] = c;
	puntajes[cantJugadores] = Puntaje();
	cantJugadores++;
}

int Equipo::cantidadDeJugadores() {
	return cantJugadores;
}

Puntaje& Equipo::getPuntaje(std::string nombre) {
	std::array<char, 100> c = toCharArray<100>(nombre);

	for (int i = 0; i < cantJugadores; i++) {
		if (fromCharArray(c) == fromCharArray(nombres[i]))
			return puntajes[i];
	}

	throw "No existe jugador en equipo";
}

HashMap<std::string, Puntaje> Equipo::getPuntajes() {
	HashMap<std::string, Puntaje> m;
	for (int i = 0; i < cantJugadores; i++) {
		std::string s = fromCharArray(nombres[i]);
		m[s] = puntajes[i];
	}

	return m;
}

