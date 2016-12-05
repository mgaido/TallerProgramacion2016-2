#include "Enemigo.h"

Enemigo::Enemigo(int id, Config& _configuracion) : Personaje(id, _configuracion) {
	velocCaminar = 0 , velocSaltoX = 0, velocSaltoY = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
	energia = 250;
	puntosQueDaAlMorir = 50;				//chequear con tp la cantidad q piden
	estado = Estado::Quieto;
	tipo = Tipo::Enemigo;
	distanciaPiso = 0;
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
	if (valor < 30) {				//30% dropear bonus al morir 
		nuevoPickup = new PickUp(++contador);
		nuevoPickup->setPos(this->pos);
	}
	return nuevoPickup;
}

int Enemigo::getPuntos() {
	return puntosQueDaAlMorir;
}

int Enemigo::getDistanciaPiso() {
	return distanciaPiso;
}

void Enemigo::setDistanciaPiso(int distancia) {
	this->distanciaPiso = distancia;
}

void Enemigo::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);

	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -configuracion.getVelocidadX() /2;
	else
		velocCaminar = configuracion.getVelocidadX() /2;

	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}

void Enemigo::saltar() {
	std::unique_lock<std::mutex> lock(mutex);

	if (tiempoSalto == 0) {
		velocSaltoY = configuracion.getVelocidadX();
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
	cambios = true;
}

void Enemigo::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos) {
	/*if ((tiempoActual - tiempoCreacion) > (10 * (rand() % 500) * 1000)) {
		this->detenerse();
	}
	if (velocCaminar == 0) {
		if ((tiempoActual - tiempoCreacion) > (6 * (rand() % 500) * 1000)) {
			proyectilesEnemigos->push_back(this->disparar());
		}
	}

	if (velocCaminar != 0) {
		if ((tiempoActual - tiempoCreacion) > (2 * (rand() % 500) * 1000)) {
			this->saltar();
		}
	}
	*/
	this->saltar();
}

void Enemigo::setTiempoCreacion(micros tiempoCreacion){
	this->tiempoCreacion = tiempoCreacion;
}
