#pragma once
#ifndef GUNC_H_
#define GUNC_H_

#include "Proyectil.h"

#define PI 3.14159265


//Misiles que siguen al Jugador
class GunC : public Proyectil {
public:
	GunC(int id);
	virtual bool tieneCambios(Juego* juego);
	virtual void setOrientacionX(bool orientacion);
	virtual EstadoObj getEstadoObj(Escenario& escenario);
private:
	Punto posEnemigoMasCercano(Juego* juego);
	micros tiempoCreacion;

	int _angle;

};

#endif /* GUNC_H_ */
