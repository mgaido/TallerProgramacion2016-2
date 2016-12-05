#pragma once
#ifndef BOMBA_H_
#define BOMBA_H_

#include "Proyectil.h"

//Arma Basica
class Bomba : public Proyectil {
public:
	Bomba(int id, int idJugador);
private:
protected:
	virtual Proyectil* crearProyectil();


};

#endif /* GUNH_H_ */
