#ifndef BOSS2_H_
#define BOSS2_H_

#include "Boss.h"

const double VELOCIDAD_BOSS = 0.0001;

class Boss2 : public Boss {
public:
	Boss2(int id, Config& _configuracion);
	virtual bool tieneCambios(Juego* juego);
};

#endif /* BOSS_H_ */
