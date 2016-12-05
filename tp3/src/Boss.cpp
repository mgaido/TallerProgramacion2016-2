#include "Boss.h"


void Boss::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos) {
	                             //Logica MOVIMIENTO BOSS HABRIA QUE MEJORAR para que no salga del mapa
	if (getVelocidadCaminar() > 0) {
		detenerse();
		caminar(Direccion::IZQUIERDA);
	}
	else {
		detenerse();
		caminar(Direccion::DERECHA);
	}
}
