#include "Boss.h"


Boss::Boss(int id, Config & _configuracion) : Enemigo(id, _configuracion) {
}

void Boss::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos)
{
}
