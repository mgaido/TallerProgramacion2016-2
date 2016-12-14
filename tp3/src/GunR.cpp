#include "GunR.h"

GunR::GunR(int id) : Proyectil(id) {
	this->danio = 80;
	this->puntosOtorgados = 40;
	this->tipo = Tipo::GunR;
	this->estado = Estado::Normal;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->cooldown = 333;
}
