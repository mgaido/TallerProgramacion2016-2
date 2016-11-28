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

PickUp* Enemigo::spawnPickUp() {
	int valor = rand() % 100;
	PickUp* nuevoPickup = NULL;
	if (valor < 200) {				//30% dropear bonus al morir 
		nuevoPickup = new PickUp(++contador);
		nuevoPickup->setPos(this->pos);
	}
	return nuevoPickup;
}

//void Enemigo::caminar(Direccion direccion) {
//	std::unique_lock<std::mutex> lock(mutex);
//
//	if (direccion == Direccion::IZQUIERDA)
//		velocCaminar = -configuracion.getVelocidadX();
//	else
//		velocCaminar = configuracion.getVelocidadX();
//
//	if (tiempoSalto == 0)
//		tiempoCaminando = 1000000;
//	std::cout << tiempo() << std::endl;
//}