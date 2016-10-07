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
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;

	tiempoCaminando=0;
	tiempoSalto=0;

	estado = Estado::Quieto;
	tipo = Tipo::Jugador;
}

Jugador::~Jugador() {
}

void Jugador::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);
	
	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -vx;
	else
		velocCaminar = vx;
	
	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}

void Jugador::detenerse() {
	std::unique_lock<std::mutex> lock(mutex);
	actualizar();
	velocCaminar = 0;
	tiempoCaminando = 0;
}

void Jugador::saltar() {
	std::unique_lock<std::mutex> lock(mutex);

	if (tiempoSalto == 0) {
		velocSaltoY=vy;
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
}

bool Jugador::tieneCambios() {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar();
}


bool Jugador::actualizar() {
	millis t;
	double vx = 0;

	if (tiempoSalto > 0) {
		vx = velocSaltoX;
		t = tiempo() - tiempoSalto;
		velocSaltoY -= t*g;
		tiempoSalto += t;
	} else if (tiempoCaminando > 0) {
		vx = velocCaminar;
		t = tiempo() - tiempoCaminando;
		tiempoCaminando += t;
	}

	bool cambios = t != 0;

	pos.x += vx*t;
	pos.y += velocSaltoY*t;
	if (tiempoSalto > 0 && pos.y <= 0) {
		pos.y = 0;
		velocSaltoX = 0;
		velocSaltoY = 0;
		if (velocCaminar != 0)
			tiempoCaminando = tiempoSalto;
		tiempoSalto = 0;
	}

	if (t > 0 && pos.x < 0)
		pos.x = 0;
	
	Estado estado;
	if (tiempoSalto > 0)
		estado = Estado::Saltando;
	else if (tiempoCaminando > 0)
		estado = Estado::Caminando;
	else
		estado = Estado::Quieto;

	cambios |= estado != this->estado;
	this->estado = estado;

	return cambios;
}

