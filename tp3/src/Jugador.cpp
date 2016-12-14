/*
 * Jugador.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Jugador.h"

Jugador::Jugador(int id, std::string nombre, Puntaje* puntaje, Config& _configuracion) : Soldado(id, _configuracion) {
	this->nombre = nombre;
	estado = Estado::Desconectado;
	tipo = Tipo::Jugador;
	energia = 500;
	inmortal = configuracion.esInmortal();
	this->puntaje = puntaje;
	this->puntaje->nuevoNivel();

	//arma.setTipo(Tipo::GunC);
}

bool Jugador::recibirDanio(int danio) {
	if (inmortal ||estado == Estado::Desconectado)
		return false;

	return Soldado::recibirDanio(danio);
}

Jugador::~Jugador() {
}

std::string Jugador::getNombre() {
	return nombre;
}

void Jugador::restablecerEnergia() {
	energia = 500;
}

void Jugador::recibirPuntos(int puntosObtenidos){
	puntaje->sumarPuntos(puntosObtenidos);
}

Puntaje* Jugador::getPuntaje() {
	return puntaje;
}

void Jugador::setConectado(bool conectado) {
	if (estado == Estado::Desconectado && conectado) {
		estado = Estado::Normal;
		frame = 0;
		cambios = true;
	}

	if (estado != Estado::Desconectado && !conectado) {
		estado = Estado::Desconectado;
		frame = 0;
		cambios = true;
	}
}

Proyectil* Jugador::disparar(Juego* juego) {
	Proyectil* p = Soldado::disparar(juego);

	if (p != nullptr && p->getTipo() == Tipo::GunH && velocCaminar != 0 && p->getVelocidadProyectilY() != 0)
		p->setVelocidadProyectilX((orientacion ? -1 : 1) * 0.0005);

	return p;
}
