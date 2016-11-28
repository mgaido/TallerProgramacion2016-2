#pragma once
#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Proyectil.h"
#include "Config.h"

class Personaje : public Objeto {
public:
	Personaje(int id, Config& _configuracion);
	int getEnergia();
	bool recibirDanio(int a); //Devuelve True si se la Energia llega a 0

	void caminar(Direccion direccion);
	void detenerse();
	void saltar();
	Proyectil* disparar();
	virtual bool tieneCambios();
	virtual bool esEnemigo() = 0;
	bool estaMuerto();

protected:
	bool actualizar();
	bool dispara;
	Config& configuracion;
	bool cambios;
	int energia;
	double velocCaminar, velocSaltoX, velocSaltoY;
	micros tiempoSalto;
	micros tiempoCaminando;
	std::mutex mutex;
	Proyectil *arma;
};

#endif /* PERSONAJE_H_ */
