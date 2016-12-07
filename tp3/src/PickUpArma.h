#ifndef PICKUPARMA_H_
#define PICKUPARMA_H_

#include "PickUp.h"

class PickUpArma : public PickUp {
public:
	PickUpArma(int id, Punto pos, Tipo tipoProyectil);
	virtual void aplicar(Juego* juego, Jugador* jugador);
private:
	Tipo tipoProyectil;
};

#endif /* PICKUPARMA_H_ */
