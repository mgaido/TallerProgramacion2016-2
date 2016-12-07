/*
 * EnemigoFuerte.h
 *
 *  Created on: 7 dic. 2016
 *      Author: Rod
 */

#ifndef ENEMIGOFUERTE_H_
#define ENEMIGOFUERTE_H_

#include "Enemigo.h"

class Boss1;

class EnemigoFuerte: public Enemigo {
public:
	EnemigoFuerte(int id, Config& _configuracion, Boss1* boss);
};

#endif /* ENEMIGOFUERTE_H_ */
