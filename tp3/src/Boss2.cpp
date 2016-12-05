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
	cambios = false;
}

void Boss2::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos)
{
}