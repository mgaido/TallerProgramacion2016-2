#pragma once
#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

#include "Objeto.h"
const int GROSOR_PLATAFORMA = 20;

class Plataforma : public Objeto {
public:
	Plataforma(int id, int x, int y, int ancho);
private:

};

#endif /* PLATAFORMA_H_ */
