#pragma once
#ifndef BOSS_H_
#define BOSS_H_

#include "Enemigo.h"


class Boss : public Enemigo {
public:
	Boss(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);

protected:
	char numeroDeBoss;
};

#endif /* BOSS_H_ */
