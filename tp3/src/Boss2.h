#ifndef BOSS2_H_
#define BOSS2_H_

#include "Boss.h"

const double VELOCIDAD_BOSS = 0.0001;

class Boss2 : public Boss {
public:
	Boss2(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);

protected:
	char numeroDeBoss;
};

#endif /* BOSS_H_ */