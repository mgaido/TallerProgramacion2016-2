#ifndef BOSS0_H_
#define BOSS0_H_

#include "Boss.h"


class Boss0 : public Boss {
public:
	Boss0(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);

protected:
	char numeroDeBoss;
};

#endif /* BOSS_H_ */