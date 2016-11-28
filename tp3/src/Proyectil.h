#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"

class Proyectil : public Objeto {
public:
	Proyectil(int id);
	void recargar();
	int getPuntos();
	void setPos(Punto nuevaPosicion);
	virtual bool tieneCambios();
	Proyectil* disparar(); //Devuelve True si se pudo disparar, False si se quedo sin Balas.
	~Proyectil();
	void trayectoria();
	void setOrientacion(bool nuevaOrientacion);
private:

protected:
	std::mutex mutex;
	virtual Proyectil* crearProyectil();
	int danio;
	int cantidad;
	int cantidadRepuesto;
	int puntosOtorgados;
	int tiempoUltimoDisparo;
	double velocidadProyectil;
	double tiempoEnMovimiento;
	bool cambios;
};

#endif /* PROYECTIL_H_ */
