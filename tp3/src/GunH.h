#pragma once
#ifndef GUNH_H_
#define GUNH_H_

#include "Proyectil.h"

//Arma Basica
class GunH : public Proyectil {
public:
	GunH(int id);
private:
protected:
	virtual Proyectil* crearProyectil();


};

#endif /* GUNH_H_ */
