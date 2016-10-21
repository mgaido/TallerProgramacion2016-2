/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

Jugador::Jugador(int id, std::string nombre, Config& _configuracion) : Objeto(id), configuracion(_configuracion) {
	this->nombre = nombre;
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;

	tiempoCaminando=0;
	tiempoSalto=0;

	estado = Estado::Desconectado;
	tipo = Tipo::Jugador;

	cambios = false;
}

Jugador::~Jugador() {
}

std::string Jugador::getNombre() {
	return nombre;
}

void Jugador::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);
	
	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = - configuracion.getVelocidadX();
	else
		velocCaminar = configuracion.getVelocidadX();
	
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
		velocSaltoY=configuracion.getVelocidadY();
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
}

bool Jugador::tieneCambios() {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar();
}

void Jugador::setConectado(bool conectado) {
	if (estado == Estado::Desconectado && conectado) {
		estado = Estado::Quieto;
		frame = 0;
		cambios = true;
	}

	if (estado != Estado::Desconectado && ! conectado) {
		estado = Estado::Desconectado;
		frame = 0;
		cambios = true;
	}
}


bool Jugador::actualizar() {
	if (estado != Estado::Desconectado) {

		micros t=0;
		double vx = 0;

		if (tiempoSalto > 0) {
			vx = velocSaltoX;
			t = tiempo() - tiempoSalto;
			velocSaltoY -= t*configuracion.getGravedad();
			tiempoSalto += t;
		} else if (tiempoCaminando > 0) {
			vx = velocCaminar;
			t = tiempo() - tiempoCaminando;
			tiempoCaminando += t;
		}

		cambios |= t != 0;

		if (vx != 0)
			orientacion = vx < 0;

		pos.x += (int) round(vx*t);
		pos.y += (int) round(velocSaltoY*t);

		if (tiempoSalto > 0 && velocSaltoY < 0 &&  pos.y <= 0) {
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

		if (estado != this->estado) {
			this->estado = estado;
			cambios = true;
			frame = 0;
		}
	}

	bool rv = cambios;
	cambios = false;
	return rv;
}
