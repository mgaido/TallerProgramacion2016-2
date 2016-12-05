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
	crearPlataformas();
	escenario = Escenario(configuracion.getLongitud(), configuracion.getTamanioVentana().x, configuracion.getNivelPiso());
	maxOffsetDelta = round(configuracion.getVelocidadX() * 2 * 1000000.0 / configuracion.getFrameRate());
	t_updateWorld = std::thread(&Juego::updateWorld, this);
	BossFinal = NULL;
}

Juego::~Juego() {
	auto it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;
		delete obj;
		jugadores.erase(it);
	}
}

void Juego::crearPlataformas() {
	std::vector<Plataformas> plataformasConfig = configuracion.getPlataformas();
	auto it = plataformasConfig.begin();
	while (it != plataformasConfig.end()) {
		Plataforma* nuevaPlataforma = new Plataforma(++contador, it->punto.x, it->punto.y, it->ancho);
		plataformas.push_back(nuevaPlataforma);
		it++;
	}
}

void Juego::updateWorld() {
	std::thread t_detenerEnemigoAnterior;
	Enemigo* enemigoSpawneado;
	while (!detenido) {
		if (BossFinal != NULL || minPosXJugador < 5500) {
			enemigoSpawneado = spawnEnemigo();
		}
		else {
			enemigoSpawneado = spawnBoss();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10 * (rand() % 500)));
		enemigoSpawneado->detenerse();
		
		if ((rand() % 7 == 0) && (enemigoSpawneado->getTipo() == Tipo::Enemigo)) //Logica para que el enemigo salte y suba a una plataforma
			enemigoSpawneado->saltar();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 * (rand() % 5)));
		if (BossFinal != NULL ) {                                 //Logica MOVIMIENTO BOSS HABRIA QUE MEJORAR para que no salga del mapa
			if (BossFinal->getVelocidadCaminar() > 0) {
				BossFinal->detenerse();
				BossFinal->caminar(Direccion::IZQUIERDA);
			}
			else {
				BossFinal->detenerse();
				BossFinal->caminar(Direccion::DERECHA);
			}
		}
	}
}

void Juego::detener() {
	if (!detenido) {
		detenido = true;
		t_updateWorld.join();
		debug("Thread updateWorld termino");
	}
}

void Juego::agregarProyectilAliado(Proyectil * nuevoProyectil){
	proyectilesAliados.push_back(nuevoProyectil);
}

void Juego::agregarProyectilEnemigo(Proyectil * nuevoProyectil) {
	proyectilesEnemigos.push_back(nuevoProyectil);
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

Enemigo* Juego::spawnBoss() {
	Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);
	int nivel = 0;
	if (nivel == 0 ) { //Se crea el Boss HI-DO
		nuevoEnemigo->getTamanio().x = 200;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 400;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->setTipo(Tipo::Boss1);
		contadorEnemigosSpawneados++;
		lock.lock();
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		nuevoEnemigo->setDistanciaPiso(-400);
		enemigos.push_back(nuevoEnemigo);

		lock.unlock();
		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss HI-DO creado");
	}
	else if (nivel == 1 && (BossFinal == NULL)) { //Se crea el Boss AirbusterRiberts
		nuevoEnemigo->getTamanio().x = 400;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 400;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->setTipo(Tipo::Boss2);
		contadorEnemigosSpawneados++;
		lock.lock();
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		nuevoEnemigo->setDistanciaPiso(-400);
		enemigos.push_back(nuevoEnemigo);

		lock.unlock();

		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss creado");
	}
	else if (nivel == 2 && (BossFinal == NULL)) { //Se crea el Boss TANI OH
		nuevoEnemigo->getTamanio().x = 400;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 400;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->setTipo(Tipo::Boss3);
		contadorEnemigosSpawneados++;
		lock.lock();
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		nuevoEnemigo->setDistanciaPiso(-180);
		enemigos.push_back(nuevoEnemigo);

		lock.unlock();

		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss creado");
	}
	return nuevoEnemigo;
}


Enemigo* Juego::spawnEnemigo(){
	Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);
		
	nuevoEnemigo->getTamanio().x = configuracion.getTamanioJugador().x;				//configurar para enemigo 
	nuevoEnemigo->getTamanio().y = configuracion.getTamanioJugador().y;				//configurar para enemigo
	nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
	nuevoEnemigo->setTipo(Tipo::Enemigo);
	nuevoEnemigo->setDistanciaPiso(-nuevoEnemigo->getTamanio().y - nuevoEnemigo->getPos().y);
	contadorEnemigosSpawneados++;
	lock.lock();
	nuevoEnemigo->caminar(Direccion::IZQUIERDA);
	enemigos.push_back(nuevoEnemigo);

	lock.unlock();

	cambios = true;

	info("Enemigo creado");
	

	return nuevoEnemigo;
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estado;

	lock.lock();

	//Actualizar posiciones de jugadores y encontrar el minimo en X
	int minX = 0;
	auto it = jugadores.begin();
	 
	for (auto it = jugadores.begin(); it != jugadores.end();) {
		Jugador* unJugador = *it;
		cambios |= unJugador->tieneCambios(plataformas);
		bool colisionan = false;
		if ((minX == 0 || unJugador->getPos().x < minX) && unJugador->getEstado() != Estado::Desconectado)
			minX = unJugador->getPos().x + unJugador->getTamanio().x / 2;
			minPosXJugador = minX;

		auto it4 = pickups.begin();
		while ((it4 != pickups.end()) && !colisionan) {
			PickUp *unPickUp = *it4;
			if (((unJugador->getPos().x + unJugador->getTamanio().x) < (unPickUp->getPos().x)) || (unJugador->getPos().x > unPickUp->getPos().x + unPickUp->getTamanio().x)) {
				colisionan = false;
			}
			else if (((unJugador->getPos().y + unJugador->getTamanio().y) < (unPickUp->getPos().y)) || (unJugador->getPos().y > unPickUp->getPos().y + unPickUp->getTamanio().y)) {
				colisionan = false;
			}
			else { colisionan = true; }

			if (colisionan) {
				unJugador->recibirBonus(unPickUp);
				//if(unJugador->getArma->getTipo() == Gunc)  buscarEnemigoMasCercano , setEnemigomas cercano
				pickups.erase(it4);
				if (unJugador->getKillAll()) {
					auto it2 = enemigos.begin();
					while (it2 != enemigos.end()) {
						Enemigo* obj = *it2;
						delete (*it2);
						enemigos.erase(it2);;
					}
				}
			}
			it4++;
		}
		it++;
	}

	auto it2 = enemigos.begin();
	while (it2 != enemigos.end()) {
		Enemigo* obj = *it2;
		cambios |= obj->tieneCambios(plataformas);
		it2++;
	}

	for(auto it3 = proyectilesAliados.begin(); it3 != proyectilesAliados.end();) {
		Proyectil* unProyectil = *it3;
		cambios |= unProyectil->tieneCambios(plataformas);
		bool colisionan = false;
		it2 = enemigos.begin();
		while ((it2 != enemigos.end()) && !colisionan) {
			Enemigo *unEnemigo = *it2;
 			if (((unProyectil->getPos().x + unProyectil->getTamanio().x) < (unEnemigo->getPos().x)) || (unProyectil->getPos().x > unEnemigo->getPos().x + unEnemigo->getTamanio().x)) {
				colisionan = false;
			}
			else if (((unProyectil->getPos().y + unProyectil->getTamanio().y) < (unEnemigo->getPos().y)) || (unProyectil->getPos().y > unEnemigo->getPos().y + unEnemigo->getTamanio().y)) {
				colisionan = false;
			}
			else { colisionan = true; }

			if (colisionan) {
				bool estaMuerto = unEnemigo->recibirDanio(unProyectil->getDanio());
				if (estaMuerto) {
					PickUp* nuevoPickup = unEnemigo->spawnPickUp();
					if (nuevoPickup != NULL)
						pickups.push_back(nuevoPickup);
					it = jugadores.begin();
					bool encontrado = false;
					while ((it != jugadores.end()) && !encontrado) {
						Jugador* jugadorQueEliminoAlEnemigo = *it;
						if(unProyectil->getIdTirador() == jugadorQueEliminoAlEnemigo->getId())
							jugadorQueEliminoAlEnemigo->recibirPuntos(unEnemigo->getPuntos());
						it++;
					}
					efectos.push_back(new EnemigoMuriendo(++contador, unEnemigo->getPos(), unEnemigo->getTamanio(), unProyectil->getTipo()));
					enemigos.erase(it2);
				}
			}
			it2++;
		}
		if(colisionan || !unProyectil->esVisible()) {
			efectos.push_back(new ImpactoBala(++contador, unProyectil));
			it3 = proyectilesAliados.erase(it3);
		} else
			it3++;

	}

	for (auto it6 = proyectilesEnemigos.begin(); it6 != proyectilesEnemigos.end();) {
		Proyectil* unProyectil = *it6;
		cambios |= unProyectil->tieneCambios(plataformas);
		bool colisionan = false;
		it = jugadores.begin();
		while ((it != jugadores.end()) && !colisionan) {
			Jugador *unJugador = *it;
			if (((unProyectil->getPos().x + unProyectil->getTamanio().x) < (unJugador->getPos().x)) || (unProyectil->getPos().x > unJugador->getPos().x + unJugador->getTamanio().x)) {
				colisionan = false;
			}
			else if (((unProyectil->getPos().y + unProyectil->getTamanio().y) < (unJugador->getPos().y)) || (unProyectil->getPos().y > unJugador->getPos().y + unJugador->getTamanio().y)) {
				colisionan = false;
			}
			else { colisionan = true; }

			if (colisionan) {
				bool estaMuerto = unJugador->recibirDanio(unProyectil->getDanio());
				if (estaMuerto) {
						//hacer algo GAME OVER? ???  ? ? ? ? ? ? ? ?? ? ? ? ? ? ?? ? ? ? ? ? ? ? ? ? ? ? ??? ? '
				}
			}
			it++;
		}
		if (colisionan || !unProyectil->esVisible())
			it6 = proyectilesEnemigos.erase(it6);
		else
			it6++;
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

	auto efectosIt = efectos.begin();
	while (efectosIt != efectos.end()) {
		Efecto* obj = *efectosIt;
		cambios |= obj->tieneCambios(plataformas);

		if (obj->esVisible())
			efectosIt++;
		else
			efectosIt = efectos.erase(efectosIt);
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
			//std::cout << obj->getPos().x << std::endl;
			//std::cout << minPosXJugador << std::endl;

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
			Enemigo* obj = *it2;

			EstadoObj estadoObj;
			estadoObj.setId(obj->getId());
			estadoObj.setTipo(obj->getTipo());
			estadoObj.setEstado(obj->getEstado());
			estadoObj.setEstado(obj->getEstado());
			estadoObj.setTipo(obj->getTipo());


			Punto pos;
			pos.x = obj->getPos().x - escenario.getOffsetVista();
			pos.y = escenario.getNivelPiso() + obj->getDistanciaPiso();
			estadoObj.setPos(pos);
			estadoObj.setTamanio(obj->getTamanio());
			estadoObj.setFrame(obj->getFrame());
			estadoObj.setOrientacion(obj->getOrientacion());
			
			estado.push_back(estadoObj);
			it2++;
		}

		auto it3 = proyectilesAliados.begin();
		while (it3 != proyectilesAliados.end()) {
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

		auto it4 = pickups.begin();
		while (it4 != pickups.end()) {
			Objeto* obj = *it4;

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
			it4++;
		}

		auto it5 = plataformas.begin();
		while (it5 != plataformas.end()) {
			Objeto* obj = *it5;

			int inicio = obj->getPos().x;
			int fin = obj->getPos().x + obj->getTamanio().x;

			if (fin >= escenario.getOffsetVista() && inicio <= escenario.getOffsetVista() + escenario.getAnchoVista()) {
				inicio = inicio - escenario.getOffsetVista();
				fin = std::max<int>(fin - escenario.getOffsetVista(), 0);
				fin = std::min<int>(escenario.getAnchoVista(), fin);

				EstadoObj estadoObj;
				estadoObj.setId(obj->getId());
				estadoObj.setTipo(obj->getTipo());
				estadoObj.setEstado(obj->getEstado());
				Punto pos;
				pos.x = inicio;
				pos.y = escenario.getNivelPiso() - obj->getPos().y;
				estadoObj.setPos(pos);
				Punto tamanio;
				tamanio.x = fin - inicio;
				tamanio.y = GROSOR_PLATAFORMA;
				estadoObj.setTamanio(tamanio);
				estadoObj.setFrame(obj->getFrame());
				estadoObj.setOrientacion(obj->getOrientacion());

				estado.push_back(estadoObj);
			}
			it5++;
		}

		auto it6 = proyectilesEnemigos.begin();
		while (it6 != proyectilesEnemigos.end()) {
			Objeto* obj = *it6;

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
			it6++;
		}

		auto efectosIt = efectos.begin();
		while (efectosIt != efectos.end()) {
			Efecto* obj = *efectosIt;

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
			estadoObj.setNombre(obj->getNombre());
			estado.push_back(estadoObj);
			efectosIt++;
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
