#include "Enemigo.h"

Enemigo::Enemigo(int id, Config& _configuracion) : Personaje(id, _configuracion) {
	velocCaminar = 0 , velocSaltoX = 0, velocSaltoY = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;

	estado = Estado::Quieto;
	tipo = Tipo::Enemigo;

	cambios = false;
}

Enemigo::~Enemigo() {

}

bool Enemigo::esEnemigo() {
	return true;
}

bool Enemigo::spawnPickUp() {
	int valor = rand() % 100;
	return (valor < 30);  //30% de prob q tire pickup al morir 
}