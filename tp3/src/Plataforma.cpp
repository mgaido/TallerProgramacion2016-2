#include "Plataforma.h"

Plataforma::Plataforma(int id, int x, int y, int ancho) : Objeto(id) {
	this->tipo = Tipo::Plataforma;
	this->estado = Estado::Quieto;
	this->tamanio.x = ancho;
	this->tamanio.y = GROSOR_PLATAFORMA;
	this->pos.x = x; 
	this->pos.y = y; 
	this->orientacion = false;
	this->frame = 0;
}