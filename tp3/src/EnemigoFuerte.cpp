/*
 * EnemigoFuerte.cpp
 *
 *  Created on: 7 dic. 2016
 *      Author: Rod
 */

#include "EnemigoFuerte.h"
#include "Boss1.h"

EnemigoFuerte::EnemigoFuerte(int id, Config& _configuracion, Boss1* boss) : Enemigo(id, _configuracion) {
	energia = 250;
	tipo = Tipo::EnemigoFuerte;
	arma.setTipo(Tipo::GunS);

	pos.x = boss->getPos().x + boss->getTamanio().x / 2 - tamanio.x / 2;
	pos.y = boss->getPos().y;
}

