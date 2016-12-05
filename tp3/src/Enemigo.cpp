#include "Enemigo.h"

Enemigo::Enemigo(int id, Config& _configuracion) : Personaje(id, _configuracion) {
	velocCaminar = 0 , velocSaltoX = 0, velocSaltoY = 0;
	aumentoDeDanio = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
	energia = 250;
	puntosQueDaAlMorir = 50;				//chequear con tp la cantidad q piden
	estado = Estado::Quieto;
	tipo = Tipo::Enemigo;
	distanciaPiso = 0;
	cambios = false;
	int valor = (rand() % 100);
	if (valor < 20)
		arma = new GunH(++contador, id);
	else if (valor < 60)
		arma = new GunS(++contador, id);
	else
		arma = NULL;
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
		velocSaltoY = 0.0009;
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
	cambios = true;
}

Proyectil * Enemigo::disparar(){
	Proyectil* nuevoProyectil = NULL;
	if (arma != NULL) {
		nuevoProyectil = arma->disparar();
	}
	if (nuevoProyectil != NULL) {
		Punto pos;
		pos.x = getPos().x + (getOrientacion() ? 0 : getTamanio().x);
		pos.y = getPos().y - getTamanio().y * 0.6;
		nuevoProyectil->setPos(pos);
		nuevoProyectil->setSiElDisparadorEstaSaltando(tiempoSalto != 0);
		nuevoProyectil->setOrientacionX(orientacion);
		nuevoProyectil->setOrientacionY(apunta);
		nuevoProyectil->aumentarDanio(aumentoDeDanio);
	}
	return nuevoProyectil;
}

void Enemigo::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos) {
	if ((tiempoActual - tiempoCreacion) > (10 * (rand() % 500) * 1000)) {
		this->detenerse();
	}
	if (velocCaminar == 0) {
		if ((tiempoActual - tiempoCreacion) > (6 * (rand() % 500) * 1000)) {
			Proyectil* nuevoProyectil = this->disparar();
			if(nuevoProyectil != NULL)
				proyectilesEnemigos->push_back(nuevoProyectil);
		}
	}

	if (velocCaminar != 0) {
		if ((tiempoActual - tiempoCreacion) > (2 * (rand() % 500) * 1000)) {
			this->saltar();
		}
	}
}

void Enemigo::setTiempoCreacion(micros tiempoCreacion){
	this->tiempoCreacion = tiempoCreacion;
}

void Enemigo::aumentarEnergia(){
	energia += 500;
}

void Enemigo::aumentarDanio(){
	aumentoDeDanio = 50;
}
