#pragma once
#ifndef PICKUP_H
#define PICKUP_H

#include "Objeto.h"
#include "GunC.h"
#include "GunH.h"
#include "GunR.h"
#include "GunS.h"

class Juego;

class PickUp : public Objeto {
public:
	PickUp(int id, Punto pos);

	virtual bool tieneCambios(Juego* juego);
	virtual void aplicar(Juego* juego, Jugador* jugador) = 0;
};

#endif // PICKUP_H
