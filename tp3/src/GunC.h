#pragma once
#ifndef GUNC_H_
#define GUNC_H_

#include "Proyectil.h"

//Misiles que siguen al Jugador
class GunC : public Proyectil {
public:
	GunC(int id);
private:
protected:
	virtual Proyectil* crearProyectil();

};

#endif /* GUNC_H_ */
