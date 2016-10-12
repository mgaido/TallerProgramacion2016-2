/*
 * Juego.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUEGO_H_
#define JUEGO_H_

#include "Actualizacion.h"
#include "Conexion.h"
#include "Jugador.h"
#include "Objeto.h"
#include "Bytes.h"
#include "Logger.h"

class Juego {
public:
	Juego();

	Jugador* nuevoJugador(std::string ip, std::string nombre);
	bool getActualizaciones(Bytes& bytes);
	bool estaIniciado();

private:
	bool iniciado;
	std::mutex lock;
	int contador;
	std::vector<Objeto*> objetos;
	std::vector<Actualizacion> actualizaciones;
};

#endif /* JUEGO_H_ */
