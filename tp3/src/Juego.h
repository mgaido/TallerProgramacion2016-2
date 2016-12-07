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
#include "Escenario.h"
#include "Bytes.h"
#include "Estado.h"
#include "Logger.h"
#include "Utils.h"

#include "Objeto.h"
#include "EnemigoFactory.h"

class Jugador;
class Enemigo;
class Objeto;
class Boss;

class Juego {
public:
	Juego(Config& configuracion);
	~Juego();

	Jugador* nuevoJugador(std::string nombre);
	bool getEstado(Bytes& bytes);
	bool estaIniciado();

	Config& getConfiguracion();
	Escenario& getEscenario();

	std::vector<Objeto*>& getObjetos();
	std::vector<Plataforma*>& getPlataformas();

	unsigned int getCantdadJugadores();
	void spawnBoss();

	void agregarObjeto(Objeto* objeto);

	EnemigoFactory* getEnemigoFactory();
	void setEnemigoFactory(EnemigoFactory* enemigoFactory);

	bool estaPerdido();
	bool estaGanado();

private:
	void crearPlataformas();

	Config& configuracion;
	Escenario escenario;

	EnemigoFactory* enemigoFactory;

	std::vector<Objeto*> objetos;
	std::vector<Objeto*> nuevosObjetos;
	std::vector<Plataforma*> plataformas;
	std::vector<Jugador*> jugadores;

	bool cambios;
	std::recursive_mutex lock;
	std::recursive_mutex nuevosLock;

	EstadoJuego estado;

	Boss* BossFinal;
};

#endif /* JUEGO_H_ */
