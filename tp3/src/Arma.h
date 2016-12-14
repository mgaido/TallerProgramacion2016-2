/*
 * Arma.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef ARMA_H_
#define ARMA_H_

#include "Enums.h"
#include "Proyectil.h"

class Arma {
public:
	Arma();

	void setTipo(Tipo tipo);
	Tipo getTipo();
	int getBalas();

	Proyectil* disparar();
private:
	Tipo tipo;
	int balas;
	int cooldown;
	micros ultimoDisparo;

};

#endif /* ARMA_H_ */
