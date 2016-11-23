#pragma once
#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Proyectil.h"
#include "Config.h"

enum class Direccion { IZQUIERDA, DERECHA };

class Personaje : public Objeto {
public:
	Personaje(int id, Config& _configuracion);
	int getEnergia();
	bool recibirDanio(int a); //Devuelve True si se la Energia llega a 0
	void cambiarArma(Proyectil*);

	void caminar(Direccion direccion);
	void detenerse();
	void saltar();

	virtual bool tieneCambios();

protected:
	bool actualizar();
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
