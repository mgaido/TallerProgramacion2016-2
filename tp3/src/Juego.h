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
#include "Plataforma.h"


class Juego {
public:
	Juego(Config& configuracion);
	~Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();
	unsigned int getCantdadJugadores();
	Enemigo* spawnEnemigo();
	void detener();
	void agregarProyectilAliado(Proyectil* nuevoProyectil);
	void agregarProyectilEnemigo(Proyectil* nuevoProyectil);

private:
	void crearPlataformas();
	bool detenido;
	Config& configuracion;
	bool iniciado;
	std::mutex lock;
	std::mutex lockColisiones;
	Escenario escenario;
	std::vector<Plataforma*> plataformas;
	std::vector<Jugador*> jugadores;
	std::vector<Proyectil*> proyectilesAliados;
	std::vector<Proyectil*> proyectilesEnemigos;
	std::vector<PickUp*> pickups;
	std::vector<Enemigo*> enemigos;
	//std::vector<Objeto*> objetos; para TP3
	bool cambios;
	std::thread t_updateWorld;	
	void updateWorld();
	int contadorEnemigosSpawneados; //Para el tema de los Boss
};

#endif /* JUEGO_H_ */
