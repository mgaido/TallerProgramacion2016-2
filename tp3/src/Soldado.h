#pragma once
#ifndef SOLDADO_H_
#define SOLDADO_H_

#include "Personaje.h"

class Soldado : public Personaje {
public:
	Soldado(int id, Config& configuracion);

	void caminar(Direccion direccion);
	void detenerse();
	void saltar();

	double getVelocidadCaminar();
	void apuntar(char direcion);

	virtual bool tieneCambios(Juego* juego);
	virtual Proyectil* disparar(Juego* juego);

protected:
	bool actualizar(Juego* juego);

	bool cambios;
	bool dispara;
	char apunta;

	double velocCaminar, velocSaltoX, velocSaltoY;
	micros tiempoSalto;
	micros tiempoCaminando;

	std::mutex mutex;

};

#endif /* SOLDADO_H_ */
