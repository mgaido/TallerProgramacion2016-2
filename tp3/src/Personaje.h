#pragma once
#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Objeto.h"


class Personaje : public Objeto {
public:
	Personaje();
	int getEnergia();
	bool recibirDanio(int a); //Devuelve True si se la Energia llega a 0
	void cambiarArma(Proyectil);
protected:
	int energia;
	Proyectil arma;
};

#endif /* PERSONAJE_H_ */
