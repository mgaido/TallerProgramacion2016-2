#ifndef BOSS1_H_
#define BOSS1_H_

#include "Boss.h"
#include "EnemigoFactory.h"

const char CANTIDAD_SOLDADOS_BOSS_0 = 15;

class Boss1 : public Boss {
public:
	Boss1(int id, Config& _configuracion);
	virtual bool tieneCambios(Juego* juego);

protected:
	bool factorySeteada;
};

#endif /* BOSS_H_ */
