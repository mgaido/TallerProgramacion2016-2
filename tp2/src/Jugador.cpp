/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

double vx = 0.25;
double vy = 1.4;
double g = 0.0045;

Jugador::Jugador(int id, std::string nombre) : Objeto(id) {
	tamanio.x=60;
	tamanio.y=100;
	velocX=0;
	velocY=0;

	tiempoCaminando=0;
	tiempoSalto=0;

	estado = Estado::Quieto;
	tipo = Tipo::Jugador;
}

Jugador::~Jugador() {
}

void Jugador::caminar(Direccion direccion) {
	lock.lock();
	if (tiempoSalto == 0) {
		if (direccion == Direccion::IZQUIERDA)
			velocX=-vx;
		else
			velocX=vx;

		tiempoCaminando=tiempo();
		lock.unlock();
	}
	lock.unlock();
}

void Jugador::detenerse() {
	lock.lock();
	if (tiempoCaminando > 0) {
		pos.x += velocX * (tiempo() - tiempoCaminando);
		cambios = true;
	}
	tiempoCaminando = 0;

	if (tiempoSalto == 0) {
		velocX=0;
		estado = Estado::Quieto;
	}
	lock.unlock();
}

void Jugador::saltar() {
	lock.lock();
	if (tiempoSalto == 0) {
		tiempoSalto = tiempo();
		velocY=vy;
		estado = Estado::Saltando;
	}
	lock.unlock();
}

bool Jugador::tieneCambios() {
	lock.lock();
	if (tiempoCaminando > 0) {
		pos.x += velocX * (tiempo() - tiempoCaminando);
		tiempoCaminando = tiempo();
		cambios = true;
	}

	if (tiempoSalto > 0) {
		long t = tiempo() - tiempoSalto;
		velocY -= t*g;
		pos.y += t*velocY;
		if (tiempoCaminando == 0 && velocX != 0)
			pos.x += t*velocX;

		if (pos.y <= 0) {
			pos.y = 0;
			tiempoSalto = 0;
			if (tiempoCaminando == 0) {
				velocX = 0;
				estado = Estado::Quieto;
			} else
				estado = Estado::Caminando;
		} else
			tiempoSalto = tiempo();

		cambios = true;
	}

	bool rv = cambios;
	cambios = false;

	lock.unlock();

	return rv;
}

