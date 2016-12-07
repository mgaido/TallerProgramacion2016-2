/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

Jugador::Jugador(int id, std::string nombre, Config& _configuracion) : Soldado(id, _configuracion) {
	this->nombre = nombre;
	puntos = 0;
	estado = Estado::Desconectado;
	tipo = Tipo::Jugador;
	energia = 1000;
	cambios = false;
	inmortal = configuracion.esInmortal();

	arma.setTipo(Tipo::GunH);
}

bool Jugador::recibirDanio(int danio) {
	if (inmortal)
		return false;

	return Soldado::recibirDanio(danio);
}

Jugador::~Jugador() {
}

std::string Jugador::getNombre() {
	return nombre;
}

void Jugador::restablecerEnergia() {
	energia = 1000;
}

void Jugador::recibirPuntos(int puntosObtenidos){
	//std::cout << puntosObtenidos << std::endl;
	puntos += puntosObtenidos;
}

int Jugador::getPuntos() {
	return puntos;
}

void Jugador::setConectado(bool conectado) {
	if (estado == Estado::Desconectado && conectado) {
		estado = Estado::Quieto;
		frame = 0;
		cambios = true;
	}

	if (estado != Estado::Desconectado && !conectado) {
		estado = Estado::Desconectado;
		frame = 0;
		cambios = true;
	}
}
