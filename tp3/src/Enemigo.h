
#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Personaje.h"
#include "PickUp.h"
#include <cstdlib>

class Enemigo : public Personaje {
public:
	Enemigo(int id, Config& _configuracion);
	~Enemigo();
	virtual bool esEnemigo();
	PickUp* spawnPickUp();
	//void caminar(Direccion direc);
	int getPuntos();
protected:
	int puntosQueDaAlMorir;
};

#endif /* ENEMIGO_H_ */
