#pragma once
#ifndef BOSS_H_
#define BOSS_H_

#include "Enemigo.h"

class Boss : public Enemigo {
public:
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos);

private:

};

#endif /* BOSS_H_ */
