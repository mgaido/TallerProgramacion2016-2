/*
 * Efecto.h
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#ifndef EFECTO_H_
#define EFECTO_H_

#include "Objeto.h"
#include "Proyectil.h"
#include "Plataforma.h"

class Efecto : public Objeto {
public:
	Efecto(int id, Punto pos, Punto tamanio, int puntos);
	virtual ~Efecto();

	bool esVisible();
	std::string getNombre();
	virtual bool tieneCambios(std::vector<Plataforma*>& plataformas);

protected:
	micros inicio;
	int duracion;
	int puntos;
};

#endif /* EFECTO_H_ */
