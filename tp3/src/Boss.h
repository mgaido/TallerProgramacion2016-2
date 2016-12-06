#pragma once
#ifndef BOSS_H_
#define BOSS_H_

#include "Enemigo.h"

const int MILISEGUNDOS = 100000;

class Boss : public Enemigo {
public:
	Boss(int id, Config& _configuracion);
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);
	virtual bool tieneCambios(std::vector<Plataforma*>& plataformas);
protected:
	std::mutex lock;

	virtual bool actualizar(std::vector<Plataforma*>& plataformas);
	int distanciaRecorrida;
	char numeroDeBoss;
};

#endif /* BOSS_H_ */
