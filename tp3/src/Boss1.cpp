#include "Boss1.h"

Boss1::Boss1(int id, Config & _configuracion) : Boss(id, _configuracion) {
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
	enemigosYaSpawneados = false;
}

void Boss1::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos){
	if (!enemigosYaSpawneados) {
		micros tiempoCreacionDeEnemigo = tiempo();
		int i = 0;
		do {
			if ((tiempo() - tiempoCreacionDeEnemigo) > 1000) {

				Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);

				nuevoEnemigo->getTamanio().x = configuracion.getTamanioJugador().x;				//configurar para enemigo 
				nuevoEnemigo->getTamanio().y = configuracion.getTamanioJugador().y;				//configurar para enemigo
				nuevoEnemigo->getPos().x = pos.x;
				nuevoEnemigo->getPos().y = pos.y;
				nuevoEnemigo->setTiempoCreacion(tiempo());
				nuevoEnemigo->setTipo(Tipo::Enemigo);
				nuevoEnemigo->setDistanciaPiso(-nuevoEnemigo->getTamanio().y - nuevoEnemigo->getPos().y);
				nuevoEnemigo->caminar(Direccion::IZQUIERDA);
				enemigos->push_back(nuevoEnemigo);
				i++;
				info("Enemigo creado");
				tiempoCreacionDeEnemigo = tiempo();
			}
		} while (i < CANTIDAD_SOLDADOS_BOSS_0);
		enemigosYaSpawneados = true;
	}
}
