#pragma once
#ifndef BOMBA_H_
#define BOMBA_H_

#include "Proyectil.h"

//Arma Basica
class Bomba : public Proyectil {
public:
	Bomba(int id);

	virtual bool tieneCambios(Juego* juego);

	virtual void setOrientacionX(bool orientacion);
};

#endif /* GUNH_H_ */
