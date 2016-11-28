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
#include "Proyectil.h"
#include "Enemigo.h"
#include "PickUp.h"



class Juego {
public:
	Juego(Config& configuracion);
	~Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();
	unsigned int getCantdadJugadores();
	void spawnEnemigo();
	void detener();
	void agregarProyectil(Proyectil* nuevoProyectil);

private:
	bool detenido;
	Config& configuracion;
	bool iniciado;
	std::mutex lock;
	Escenario escenario;
	std::vector<Jugador*> jugadores;
	std::vector<Proyectil*> proyectiles;
	std::vector<PickUp*> pickups;
	std::vector<Enemigo*> enemigos;
	//std::vector<Objeto*> objetos; para TP3
	bool cambios;
	std::thread t_updateWorld;
	std::thread t_chequearColisiones;
	void chequearColisiones();
	void updateWorld();
	int contadorEnemigosSpawneados; //Para el tema de los Boss
};

#endif /* JUEGO_H_ */
