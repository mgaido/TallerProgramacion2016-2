/*
 * Jugador.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include "Logger.h"
#include "Personaje.h"

class Jugador : public Personaje {
public:
	Jugador(int id, std::string nombre, Config& configuracion);
	~Jugador();
	void cambiarArma(Proyectil*);
	virtual bool esEnemigo();
		
	void setConectado(bool conectado);
	std::string getNombre();


private:
	std::string nombre;
};

#endif /* JUGADOR_H_ */
