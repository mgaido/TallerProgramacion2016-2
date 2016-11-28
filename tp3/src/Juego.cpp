/*
 * Juego.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Juego.h"

int maxOffsetDelta;
int contador = 0;

Juego::Juego(Config& _configuracion) : configuracion(_configuracion) {
	iniciado = false;
	cambios = true;
	detenido = false;
	contadorEnemigosSpawneados = 0;
	escenario = Escenario(configuracion.getLongitud(), configuracion.getTamanioVentana().x, configuracion.getNivelPiso());
	maxOffsetDelta = round(configuracion.getVelocidadX() * 2 * 1000000.0 / configuracion.getFrameRate());
	t_chequearColisiones = std::thread(&Juego::chequearColisiones, this);
}

Juego::~Juego() {
	auto it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;
		delete obj;
		jugadores.erase(it);
	}
}

void Juego::chequearColisiones() {
	while (!detenido) {
		//tal vez haa q meter lock aca
		//esto spawnea enemigos por ahora por mas del nombre q tiene
		spawnEnemigo();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 * (rand() % 7)));

	}
}

void Juego::detener() {
	if (!detenido) {
		detenido = true;
		t_chequearColisiones.join();
		debug("Thread chequearColisiones termino");
	}
}

unsigned int Juego::getCantdadJugadores(){
	return jugadores.size();
}

bool Juego::estaIniciado() {
	return iniciado;
}

Jugador* Juego::nuevoJugador(std::string nombre) {
	Jugador* jugador = new Jugador(++contador, nombre, configuracion);
	jugador->getTamanio().x = configuracion.getTamanioJugador().x;
	jugador->getTamanio().y = configuracion.getTamanioJugador().y;
	jugador->getPos().x = escenario.getOffsetVista();
	lock.lock();
	jugadores.push_back(jugador);

	if (! iniciado && jugadores.size() >= configuracion.getCantidadMinimaJugadores()) { //config
		iniciado = true;
		for (auto it = jugadores.begin(); it != jugadores.end(); it++)
			(*it)->setConectado(true);
	}
	cambios = true;
	lock.unlock();

	info("Jugador '" + nombre + "' creado");

	return jugador;
}

void Juego::spawnEnemigo(){
	if (contadorEnemigosSpawneados % 5 != 4) {
		Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);
		nuevoEnemigo->getTamanio().x = configuracion.getTamanioJugador().x;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = configuracion.getTamanioJugador().y;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->setTipo(Tipo::Enemigo);
		contadorEnemigosSpawneados++;
		lock.lock();
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		enemigos.push_back(nuevoEnemigo);

		lock.unlock();

		cambios = true;

		info("Enemigo creado");
	}
	else { //Se crea el Boss
		Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);
		nuevoEnemigo->getTamanio().x = 150;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 300;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->setTipo(Tipo::Boss);
		contadorEnemigosSpawneados++;
		lock.lock();
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		enemigos.push_back(nuevoEnemigo);

		lock.unlock();

		cambios = true;

		info("Boss creado");
	}
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estado;

	lock.lock();

	//Actualizar posiciones de jugadores y encontrar el minimo en X
	int minX = 0;
	auto it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;
		cambios |= obj->tieneCambios();

		if ((minX == 0 || obj->getPos().x < minX) && obj->getEstado() != Estado::Desconectado)
			minX = obj->getPos().x + obj->getTamanio().x / 2;
		it++;
	}
	
	auto it2 = enemigos.begin();
	while (it2 != enemigos.end()) {
		Enemigo* obj = *it2;
		cambios |= obj->tieneCambios();
		it2++;
	}

	auto it3 = proyectiles.begin();
	while (it3 != proyectiles.end()) {
		Proyectil* obj = *it3;
		cambios |= obj->tieneCambios();
		it3++;
	}

	//Actualizar el offset si es necesario
	if (minX > escenario.getOffsetVista() + escenario.getAnchoVista()/2) {
		cambios = true;
		int offset = std::min<int>(minX - escenario.getAnchoVista()/2, escenario.getOffsetVista() + maxOffsetDelta);
		offset = std::min<int>(offset, escenario.getLongitud());
		escenario.setOffsetVista(offset);
	}

	//Actualizar posiciones de jugadores para que no se salgan de pantalla
	it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;

		if ((obj->getPos().x + obj->getTamanio().x) > (escenario.getOffsetVista() + escenario.getAnchoVista())) {
			cambios = true;
			obj->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista()- obj->getTamanio().x;
		}

		if (obj->getPos().x < escenario.getOffsetVista()) {
			cambios = true;
			obj->getPos().x = escenario.getOffsetVista();
		}
		it++;
	}

	//Generar estado para la vista
	if (cambios) {
		it = jugadores.begin();
		while (it != jugadores.end()) {
			Objeto* obj = *it;

			EstadoObj estadoObj;
			estadoObj.setId(obj->getId());
			estadoObj.setTipo(obj->getTipo());
			estadoObj.setEstado(obj->getEstado());
			estadoObj.setTipo(Tipo::Jugador);
			Punto pos;
			pos.x = obj->getPos().x - escenario.getOffsetVista();
			pos.y = escenario.getNivelPiso() - obj->getTamanio().y - obj->getPos().y;
			estadoObj.setPos(pos);
			estadoObj.setTamanio(obj->getTamanio());
			estadoObj.setFrame(obj->getFrame());
			estadoObj.setOrientacion(obj->getOrientacion());

			if (obj->getTipo() == Tipo::Jugador)
				estadoObj.setNombre(((Jugador*)obj)->getNombre());

			estado.push_back(estadoObj);
			it++;
		}

		it2 = enemigos.begin();
		while (it2 != enemigos.end()) {
			Objeto* obj = *it2;

			EstadoObj estadoObj;
			estadoObj.setId(obj->getId());
			estadoObj.setTipo(obj->getTipo());
			estadoObj.setEstado(obj->getEstado());
			estadoObj.setEstado(obj->getEstado());
			estadoObj.setTipo(obj->getTipo());


			Punto pos;
			pos.x = obj->getPos().x - escenario.getOffsetVista();
			pos.y = escenario.getNivelPiso() - obj->getTamanio().y - obj->getPos().y;
			estadoObj.setPos(pos);
			estadoObj.setTamanio(obj->getTamanio());
			estadoObj.setFrame(obj->getFrame());
			estadoObj.setOrientacion(obj->getOrientacion());
			
			estado.push_back(estadoObj);
			it2++;
		}

		it3 = proyectiles.begin();
		while (it3 != proyectiles.end()) {
			Objeto* obj = *it3;

			EstadoObj estadoObj;
			estadoObj.setId(obj->getId());
			estadoObj.setTipo(obj->getTipo());
			estadoObj.setEstado(obj->getEstado());
			Punto pos;
			pos.x = obj->getPos().x - escenario.getOffsetVista();
			pos.y = escenario.getNivelPiso() - obj->getTamanio().y - obj->getPos().y;
			estadoObj.setPos(pos);
			estadoObj.setTamanio(obj->getTamanio());
			estadoObj.setFrame(obj->getFrame());
			estadoObj.setOrientacion(obj->getOrientacion());

			estado.push_back(estadoObj);
			it3++;
		}
	}

	rv = cambios;
	cambios = false;
	lock.unlock();

	if (rv) {
		bytes.put(escenario.getOffsetVista());
		bytes.putAll(estado);
	}
	return rv;
}
