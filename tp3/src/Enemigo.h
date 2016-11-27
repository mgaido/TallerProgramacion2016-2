
#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Personaje.h"
#include <cstdlib>

class Enemigo : public Personaje {
public:
	Enemigo(int id, Config& _configuracion);
	~Enemigo();
	virtual bool esEnemigo();
	bool spawnPickUp();

private:
	
};

#endif /* ENEMIGO_H_ */
