#pragma once
#ifndef GUNS_H_
#define GUNS_H_

#include "Proyectil.h"

//ShotGun
class GunS : public Proyectil {
public:
	GunS(int id);
private:
protected:
	virtual Proyectil* crearProyectil();


};

#endif /* GUNS_H_ */
