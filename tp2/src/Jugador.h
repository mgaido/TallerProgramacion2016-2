/*
 * Jugador.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include "Logger.h"
#include "Objeto.h"

enum class Direccion { IZQUIERDA, DERECHA };

class Jugador : public Objeto {
public:
	Jugador(int id, std::string nombre);
	~Jugador();

	void caminar(Direccion direccion);
	void detenerse();
	void saltar();

	virtual bool tieneCambios();

private:
	std::string nombre;
	double velocX, velocY;
	millis tiempoSalto;
	millis tiempoCaminando;
	std::mutex lock;
};

#endif /* JUGADOR_H_ */
