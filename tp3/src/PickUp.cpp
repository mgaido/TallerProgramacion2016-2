#include "PickUp.h"

PickUp::PickUp(int id) : Objeto(id) {
	this->estado = Estado::Bonus;
	this->tamanio.x = 7;
	this->tamanio.y = 7;
	this->orientacion = false;
	this->frame = 0;
	energiaACurar = 0;
	int valor = rand() % 6;
	switch (valor) {
	case (int) Bonus::GunH:
		arma = new GunH(++contador);
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunC:
		arma = new GunC(++contador);
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunS:
		arma = new GunS(++contador);
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::GunR:
		arma = new GunR(++contador);
		this->tipo = Tipo::BonusArma;
		break;
	case (int)Bonus::Vida:
		arma = NULL;
		this->tipo = Tipo::BonusVida;
		energiaACurar = 100;
		break;
	case(int)Bonus::KillAll:
		this->tipo = Tipo::BonusKill;
		arma = NULL;
		break;
	}
}


int PickUp::getEnergiaACurar() {
	return energiaACurar;
}
Proyectil* PickUp::getArma() {
	return arma;
}

void PickUp::setPos(Punto pos) {
	this->pos.x = pos.x;
	this->pos.y = 120;
}