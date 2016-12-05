/*
 * ImpactoBala.h
 *
 *  Created on: 4 dic. 2016
 *      Author: Rod
 */

#ifndef IMPACTOBALA_H_
#define IMPACTOBALA_H_

#include "Efecto.h"

class ImpactoBala: public Efecto {
public:
	ImpactoBala(int id, Proyectil* proyectil);
};

#endif /* IMPACTOBALA_H_ */
