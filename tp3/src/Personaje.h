#pragma once
#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Arma.h"
#include "Proyectil.h"
#include "Config.h"
#include "Plataforma.h"

const char UP = 0;
const char DOWN = 1;
const char NEUTRO = 2;

class Personaje : public Objeto {
public:
	Personaje(int id, Config& _configuracion);
	int getEnergia();
	virtual bool recibirDanio(int a); //Devuelve True si se la Energia llega a 0

	virtual Proyectil* disparar(Juego* juego);
	virtual Efecto* efectoAlDesaparecer();

	virtual bool tieneCambios(Juego* juego) = 0;

	Arma& getArma();

protected:
	Arma arma;
	int energia;

	Config& configuracion;
};

#endif /* PERSONAJE_H_ */
