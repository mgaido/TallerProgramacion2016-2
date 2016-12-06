#include "Boss0.h"

Boss0::Boss0(int id, Config & _configuracion) : Boss(id, _configuracion) {
	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;
	aumentoDeDanio = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
	energia = 250;
	puntosQueDaAlMorir = 50;				//chequear con tp la cantidad q piden
	estado = Estado::Quieto;
	tipo = Tipo::Enemigo;
	distanciaPiso = 0;
	cambios = false;
	arma = new GunS(++contador, id);
	tiempoUltimaBomba = 0;
	tiempoUltimoDisparo = 0;
	this->getArma()->setCantidad(1000000);
	distanciaPiso = 200;
}



void Boss0::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos) {
	if ((tiempoActual - tiempoUltimoDisparo) > (6 * (rand() % 500) * 1000)) {
 		Proyectil* nuevoProyectil = this->disparar(Direccion::IZQUIERDA);
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		nuevoProyectil = this->disparar(Direccion::DERECHA);
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		tiempoUltimoDisparo = tiempo();
	}

	if ((tiempo() - tiempoUltimaBomba) > (TIEMPO_ENTRE_BOMBAS * 1000)) {
		Proyectil* nuevoProyectil = this->dispararBomba();
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));		
		nuevoProyectil = this->dispararBomba();
		if (nuevoProyectil != NULL)
			proyectilesEnemigos->push_back(nuevoProyectil);
		tiempoUltimaBomba = tiempo();
	}
}

Proyectil * Boss0::disparar(Direccion direccion){
	Proyectil* nuevoProyectil = NULL;
	if (arma != NULL) {
		nuevoProyectil = arma->disparar();
	}
	if (nuevoProyectil != NULL) {
		Punto pos;
		pos.x = getPos().x + (getOrientacion() ? 0 : getTamanio().x);
		pos.y = getPos().y - getTamanio().y * 0.6;
		nuevoProyectil->setPos(pos);
		nuevoProyectil->setSiElDisparadorEstaSaltando(true);
		nuevoProyectil->setOrientacionY(1);
		nuevoProyectil->setOrientacionX(direccion == Direccion::IZQUIERDA);
		nuevoProyectil->aumentarDanio(aumentoDeDanio);
	}
	return nuevoProyectil;
}

Proyectil * Boss0::dispararBomba()
{
	Proyectil* nuevoProyectil = new Bomba(++contador, id);
	if (nuevoProyectil != NULL) {
		Punto pos;
		pos.x = getPos().x + (getOrientacion() ? 0 : getTamanio().x);
		pos.y = getPos().y - getTamanio().y * 0.6;
		nuevoProyectil->setPos(pos);
		nuevoProyectil->setSiElDisparadorEstaSaltando(true);
		nuevoProyectil->setOrientacionY(1);   //apunta abajo
	}
	return nuevoProyectil;
}
