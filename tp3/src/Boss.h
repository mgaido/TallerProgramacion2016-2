#pragma once
#ifndef BOSS_H_
#define BOSS_H_

#include "Personaje.h"


class Boss : public Personaje {
public:
	Boss(int id, Config& _configuracion);
	virtual bool tieneCambios(Juego* juego);
protected:
	double velocidadX;
	micros tiempoMovimiento;
	micros tiempoUltimoDisparo;

};

#endif /* BOSS_H_ */
