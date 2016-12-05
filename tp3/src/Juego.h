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
#include "ImpactoBala.h"
#include "EnemigoMuriendo.h"
#include "Efecto.h"
#include "Boss.h"
#include "Boss0.h"
#include "Boss1.h"
#include "Boss2.h"

const char MODO_INDIVIDUAL = 0;
const char MODO_COOP = 1;
const char MODO_GRUPAL = 2;
const char MODO_DIOS = 3;

class Juego {
public:
	Juego(Config& configuracion);
	~Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();
	unsigned int getCantdadJugadores();
	Enemigo* spawnEnemigo(micros tiempoCreacion);
	Enemigo* spawnBoss(micros tiempoCreacion);

	void detener();
	void agregarProyectilAliado(Proyectil* nuevoProyectil);
	void agregarProyectilEnemigo(Proyectil* nuevoProyectil);

private:
	char modoDeJuego; 
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
	std::vector<Efecto*> efectos;
	Enemigo* BossFinal;
	bool cambios;
	std::thread t_updateWorld;	
	void updateWorld();
	std::thread t_updateIA;
	void updateIA();
	std::thread t_chequearGameOver;
	void chequearGameOver();
	int contadorEnemigosSpawneados; //Para el tema de los Boss
	int minPosXJugador;
	int minPosXEnemigo;

};

#endif /* JUEGO_H_ */
