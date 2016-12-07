#include "PickUpArma.h"
#include "Juego.h"
#include "Jugador.h"

PickUpArma::PickUpArma(int id, Punto pos, Tipo tipoProyectil) : PickUp(id, pos) {
	this->tipoProyectil = tipoProyectil;
	switch (tipoProyectil) {
	case Tipo::GunH:
		this->tipo = Tipo::BonusArmaH;
		break;
	case Tipo::GunS:
		this->tipo = Tipo::BonusArmaS;
		break;
	case Tipo::GunC:
		this->tipo = Tipo::BonusArmaC;
		break;
	case Tipo::GunR:
		this->tipo = Tipo::BonusArmaR;
		break;
	default:
		break;
	}
}

void PickUpArma::aplicar(Juego* juego, Jugador* jugador) {
	jugador->getArma().setTipo(tipoProyectil);
}
