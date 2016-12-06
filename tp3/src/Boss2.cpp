#include "Boss2.h"

Boss2::Boss2(int id, Config & _configuracion) : Boss(id, _configuracion) {
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;
	aumentoDeDanio = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
	energia = 250;
	puntosQueDaAlMorir = 50;				//chequear con tp la cantidad q piden
	estado = Estado::Quieto;
	tipo = Tipo::Enemigo;
	distanciaPiso = 0;
	this->getArma()->setCantidad(1000000);
	cambios = false;
}

void Boss2::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos){
	if ((tiempoActual - tiempoUltimoDisparo) > (6 * (rand() % 500) * 1000)) {
		Proyectil* nuevoProyectil = this->disparar();
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		nuevoProyectil = this->disparar();
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		tiempoUltimoDisparo = tiempo();
	}
}

void Boss2::caminar(Direccion direccion){
	std::unique_lock<std::mutex> lock(mutex);

	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -VELOCIDAD_BOSS;
	else
		velocCaminar = VELOCIDAD_BOSS;

	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}
