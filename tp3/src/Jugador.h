/*
 * Jugador.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include "Logger.h"
#include "Soldado.h"
#include "PickUp.h"

#include "Juego.h"

class Jugador : public Soldado {
public:
	Jugador(int id, std::string nombre, Config& configuracion);
	~Jugador();

	void restablecerEnergia();
	virtual std::string getNombre();
	int getPuntos();
	void recibirPuntos(int puntosObtenidos);
	void setConectado(bool conectado);

	void cambiarArma(Proyectil*);
	virtual bool esEnemigo();
private:
	std::string nombre;
	int puntos;
};

#endif /* JUGADOR_H_ */
