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
#include "PickUp.h"

class Jugador : public Personaje {
public:
	Jugador(int id, std::string nombre, Config& configuracion);
	~Jugador();
	void cambiarArma(Proyectil*);
	virtual bool esEnemigo();
	bool getKillAll();
	void setConectado(bool conectado);
	std::string getNombre();
	void recibirBonus(PickUp* unPickUp);
	void recibirPuntos(int puntosObtenidos);
	int getPuntos();
private:
	bool killAll;
	std::string nombre;
	int puntos;
};

#endif /* JUGADOR_H_ */
