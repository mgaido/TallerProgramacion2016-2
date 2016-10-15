/*
 * Juego.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUEGO_H_
#define JUEGO_H_

#include "Conexion.h"
#include "Jugador.h"
#include "Objeto.h"
#include "Bytes.h"
#include "Estado.h"
#include "Logger.h"

class Juego {
public:
	Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();

private:
	bool iniciado;
	std::mutex lock;
	int contador;
	std::vector<Objeto*> objetos;
	bool cambios;
};

#endif /* JUEGO_H_ */
