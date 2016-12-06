#pragma once
#ifndef PICKUP_H
#define PICKUP_H

#include "Objeto.h"
#include "GunC.h"
#include "GunH.h"
#include "GunR.h"
#include "GunS.h"

enum class Bonus {GunH, GunS, GunR, Vida, KillAll, GunC};
class PickUp : public Objeto {
public:
	PickUp(int id);
	void setPos(Punto pos);
	int getEnergiaACurar();
	Bonus getBonus();
private:
	Bonus bonus;
	int energiaACurar;
};

#endif // PICKUP_H
