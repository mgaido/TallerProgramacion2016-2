#include "PickUp.h"

PickUp::PickUp(int id) : Objeto(id) {
	this->tipo = Tipo::Jugador;
	this->estado = Estado::Quieto;
	this->tamanio.x = 10;
	this->tamanio.y = 10;
	this->orientacion = false;
	this->frame = 0;
	energiaACurar = 0;
	int valor = rand() % 6;
	switch (valor) {
	case (int) Bonus::GunH:
		arma = new GunH(++contador);
		break;
	case (int)Bonus::GunC:
		arma = new GunC(++contador);
		break;
	case (int)Bonus::GunS:
		arma = new GunS(++contador);
		break;
	case (int)Bonus::GunR:
		arma = new GunR(++contador);
		break;
	case (int)Bonus::Vida:
		arma = NULL;				
		energiaACurar = 100;
		break;
	case(int)Bonus::KillAll:
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