#pragma once
#ifndef GUNF_H_
#define GUNF_H_

#include "Proyectil.h"

//LanzaLlamas
class GunF : public Proyectil {
public:
	GunF(int id);
private:
protected:
	virtual Proyectil* crearProyectil();


};

#endif /* GUNF_H_ */
