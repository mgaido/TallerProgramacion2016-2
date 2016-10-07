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
	bool actualizar();

	std::string nombre;
	double velocCaminar, velocSaltoX, velocSaltoY;
	millis tiempoSalto;
	millis tiempoCaminando;
	std::mutex mutex;
};

#endif /* JUGADOR_H_ */