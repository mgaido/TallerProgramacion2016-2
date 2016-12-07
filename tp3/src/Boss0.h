#ifndef BOSS0_H_
#define BOSS0_H_

#include "Boss.h"
#include "Bomba.h"

const micros TIEMPO_ENTRE_BOMBAS =  5000;
const micros BOMBAS_POR_VEZ =  2;


class Boss0 : public Boss {
public:
	Boss0(int id, Config& _configuracion);
	virtual Proyectil* disparar(Juego* juego);
private:
	micros tiempoUltimaBomba;
	int bombas;
};

#endif /* BOSS_H_ */
