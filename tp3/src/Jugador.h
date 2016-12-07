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

	virtual bool recibirDanio(int a);
	void restablecerEnergia();

	int getPuntos();
	void recibirPuntos(int puntosObtenidos);

	virtual std::string getNombre();

	void setConectado(bool conectado);
private:
	std::string nombre;
	int puntos;
	bool inmortal;
};

#endif /* JUGADOR_H_ */
