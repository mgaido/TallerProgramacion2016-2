/*
 * Escenario.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include "Enums.h"

class Escenario {
public:
	Escenario();
	Escenario(int longitud, int anchoVista, int altoVista, int nivelPiso);

	int getLongitud() const;
	int getAltoVista() const;
	int getAnchoVista() const;
	int getNivelPiso() const;
	int getOffsetVista() const;
	void setOffsetVista(int offsetVista);
	int getFrame();
private:
	int longitud;
	int altoVista;
	int anchoVista;
	int nivelPiso;
	int offsetVista;
};

#endif /* ESCENARIO_H_ */
