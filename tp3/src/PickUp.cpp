#include "PickUp.h"

PickUp::PickUp(int id) : Objeto(id) {
	this->estado = Estado::Bonus;
	this->tamanio.x = 70;
	this->tamanio.y = 70;
	this->orientacion = false;
	this->frame = 0;
	energiaACurar = 0;
	int valor = rand() % 6;
	bonus = (Bonus)valor;
  	switch (valor) {
	case (int) Bonus::GunH:
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunC:
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunS:
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunR:
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::Vida:
		this->tipo = Tipo::BonusVida;
		energiaACurar = 1000;
		break;
	case(int)Bonus::KillAll:
		this->tipo = Tipo::BonusKill;
		break;
	}
}

Bonus PickUp::getBonus() {
	return bonus;
}

int PickUp::getEnergiaACurar() {
	return energiaACurar;
}

void PickUp::setPos(Punto pos) {
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}