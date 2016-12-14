#include "GunS.h"

GunS::GunS(int id) : Proyectil(id) {
	this->danio = 50;
	this->puntosOtorgados = 35;
	this->tipo = Tipo::GunS;
	this->estado = Estado::Normal;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->cooldown = 250;
}
