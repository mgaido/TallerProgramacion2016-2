/*
 * Juego.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUEGO_H_
#define JUEGO_H_

#include "Conexion.h"
#include "Config.h"
#include "Jugador.h"
#include "Objeto.h"
#include "Bytes.h"
#include "Estado.h"
#include "Logger.h"
#include "Utils.h"

class Juego {
public:
	Juego(Config& configuracion);
	~Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();
	int getCantdadJugadores();

private:
	Config& configuracion;
	bool iniciado;
	std::mutex lock;
	int contador;
	Escenario escenario;
	std::vector<Jugador*> jugadores;
	//std::vector<Objeto*> objetos; para TP3
	bool cambios;
};

#endif /* JUEGO_H_ */
