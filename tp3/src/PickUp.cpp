#include "PickUp.h"
#include "Juego.h"

PickUp::PickUp(int id, Punto pos) : Objeto(id) {
	this->pos = pos;
	this->estado = Estado::Normal;
	this->tamanio.x = 70;
	this->tamanio.y = 70;
	this->orientacion = false;
}

bool PickUp::tieneCambios(Juego* juego) {
	auto it = juego->getObjetos().begin();

	while (it != juego->getObjetos().end()) {
		Objeto* obj = *it;
		if (obj->getTipo() == Tipo::Jugador) {
			if (colisionaCon(obj)) {
				aplicar(juego, (Jugador*) obj);
				visible = false;
				return true;
			}
		}
		it++;
	}
	return false;
}
