#pragma once
#ifndef GUNR_H_
#define GUNR_H_

#include "Proyectil.h"

//Lanzamisiles
class GunR : public Proyectil {
public:
	GunR(int id);
private:
protected:
	virtual Proyectil* crearProyectil();


};

#endif /* GUNR_H_ */
