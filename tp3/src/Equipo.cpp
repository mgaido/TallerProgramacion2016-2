/*
 * Equipo.cpp
 *
 *  Created on: 13 dic. 2016
 *      Author: Rod
 */

#include "Equipo.h"


Equipo::Equipo() {
}

Equipo::~Equipo() {/*
	auto it = puntajes.begin();
	while (it != puntajes.end()) {
		Puntaje* puntaje = it->second;
		delete puntaje;
		it++;
	}
	puntajes.clear();*/
}


bool Equipo::tieneJugador(std::string nombre) {
	return puntajes.find(nombre) != puntajes.end();
}

void Equipo::agregarJugador(std::string nombre) {
	puntajes[nombre] = Puntaje();
}

int Equipo::cantidadDeJugadores() {
	return puntajes.size();
}

Puntaje& Equipo::getPuntaje(std::string nombre) {
	return puntajes[nombre];
}

HashMap<std::string, Puntaje>& Equipo::getPuntajes() {
	return puntajes;
}

void Equipo::toBytes(Bytes &bytes) {
	std::vector<std::array<char, 100>> nombres;
	std::vector<Puntaje> puntos;

	for (auto it = puntajes.begin(); it != puntajes.end(); it++) {
		std::string nombre = it->first;
		std::array<char, 100> n;
		nombre.copy(n.data(), n.size());
		n[std::min<int>(nombre.size(), n.size()-1)] = '\0';
		nombres.push_back(n);
		puntos.push_back(it->second);
	}

	bytes.putAll(nombres);
	bytes.putAll(puntos);
}

void Equipo::fromBytes(Bytes &bytes) {
	std::vector<std::array<char, 100>> nombres;
	std::vector<Puntaje> puntos;

	bytes.getAll(nombres);
	bytes.getAll(puntos);

	for (int i = 0; i < nombres.size(); i++) {
		std::string nombre = std::string(nombres[i].data());
		puntajes[nombre] = puntos[i];
	}
}


