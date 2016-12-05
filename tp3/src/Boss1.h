#ifndef BOSS1_H_
#define BOSS1_H_

#include "Boss.h"
const char CANTIDAD_SOLDADOS_BOSS_0 = 15;

class Boss1 : public Boss {
public:
	Boss1(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);

protected:
	bool enemigosYaSpawneados;
	char numeroDeBoss;
};

#endif /* BOSS_H_ */