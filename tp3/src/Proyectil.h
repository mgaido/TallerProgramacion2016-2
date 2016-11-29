#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"
#include "Plataforma.h"

class Proyectil : public Objeto {
public:
	Proyectil(int id);
	void recargar();
	int getPuntos();
	void setPos(Punto nuevaPosicion);
	virtual bool tieneCambios(std::vector<Plataforma*>* plataformas);
	Proyectil* disparar(); //Devuelve True si se pudo disparar, False si se quedo sin Balas.
	~Proyectil();
	void trayectoria(std::vector<Plataforma*>* plataformas);
	void setOrientacion(bool nuevaOrientacion);
	int getDanio();
	bool esVisible();
private:

protected:
	std::mutex mutex;
	bool visible;
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
