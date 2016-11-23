#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"

class Proyectil : public Objeto {
public:
	Proyectil(int id);
	void recargar();
	int getPuntos();
	bool disparar(); //Devuelve True si se pudo disparar, False si se quedo sin Balas.
	~Proyectil();
private:

protected:
	int danio;
	int cantidad;
	int cantidadRepuesto;
	int puntosOtorgados;

};

#endif /* PROYECTIL_H_ */
