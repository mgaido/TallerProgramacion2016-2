#ifndef BOSS0_H_
#define BOSS0_H_

#include "Boss.h"
#include "Bomba.h"

const micros TIEMPO_ENTRE_BOMBAS =  20000;

class Boss0 : public Boss {
public:
	Boss0(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);
	Proyectil* disparar(Direccion direccion);

protected:
	Proyectil* dispararBomba();
	micros tiempoUltimaBomba;
	char numeroDeBoss;
};

#endif /* BOSS_H_ */