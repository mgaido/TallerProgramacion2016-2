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
	modoDeJuego = MODO_INDIVIDUAL;
	escenario = Escenario(configuracion.getLongitud(), configuracion.getTamanioVentana().x, configuracion.getNivelPiso());
	maxOffsetDelta = round(configuracion.getVelocidadX() * 2 * 1000000.0 / configuracion.getFrameRate());
	t_updateWorld = std::thread(&Juego::updateWorld, this);
	t_updateIA = std::thread(&Juego::updateIA, this);
	t_chequearGameOverYComienzoDeJuego = std::thread(&Juego::chequearGameOverYComienzoDeJuego, this);
	BossFinal = NULL;
	bossEliminado = false;
	nivel = 0;
	elJuegoEmpezo = false;
	ultimoMinX = 0;
}

Juego::~Juego() {
	auto it = jugadores.begin();
	while (it != jugadores.end()) {
		Jugador* obj = *it;
		delete obj;
		jugadores.erase(it);
	}

	auto it7 = jugadoresEnJuego.begin();
	while (it7 != jugadoresEnJuego.end()) {
		Jugador* obj = *it7;
		delete obj;
		jugadoresEnJuego.erase(it7);
	}

	auto it1 = enemigos.begin();
	while (it1 != enemigos.end()) {
		Enemigo* obj = *it1;
		delete obj;
		enemigos.erase(it1);
	}

	auto it2 = proyectilesAliados.begin();
	while (it2 != proyectilesAliados.end()) {
		Proyectil* obj = *it2;
		delete obj;
		proyectilesAliados.erase(it2);
	}

	auto it3 = proyectilesEnemigos.begin();
	while (it3 != proyectilesEnemigos.end()) {
		Proyectil* obj = *it3;
		delete obj;
		proyectilesEnemigos.erase(it3);
	}

	auto it4 = pickups.begin();
	while (it4 != pickups.end()) {
		PickUp* obj = *it4;
		delete obj;
		pickups.erase(it4);
	}

	auto it5 = plataformas.begin();
	while (it5 != plataformas.end()) {
		Plataforma* obj = *it5;
		delete obj;
		plataformas.erase(it5);
	}

	auto it6 = efectos.begin();
	while (it6 != efectos.end()) {
		Efecto* obj = *it6;
		delete obj;
		efectos.erase(it6);
	}
}

void Juego::gameOver(bool juegoGanado){
	if(juegoGanado){
		//pantalla de felicitaciones
	} else {
		//pantalla de game over 
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

void Juego::updateIA() {
	while (!detenido) {
		auto it = enemigos.begin();
		while (it != enemigos.end()) {
			Enemigo *unEnemigo = *it;
			if (unEnemigo != NULL) {
				unEnemigo->comportamiento(tiempo(), &proyectilesEnemigos, &enemigos);
				
					if ((unEnemigo->getTipo() == Tipo::Boss1) || (unEnemigo->getTipo() == Tipo::Boss2) || (unEnemigo->getTipo() == Tipo::Boss3)) {
						if (unEnemigo->getPos().x < (escenario.getOffsetVista() + unEnemigo->getTamanio().x)) {
							unEnemigo->caminar(Direccion::DERECHA);
						}
						else if (unEnemigo->getPos().x > (escenario.getOffsetVista() + escenario.getAnchoVista() - unEnemigo->getTamanio().x)) {
							unEnemigo->caminar(Direccion::IZQUIERDA);
						}
					}
			}
			it++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));				//lo duermo para q no consuma tanto recurso
	}
}

void Juego::chequearGameOverYComienzoDeJuego() {
	while (!detenido) {
		if (elJuegoEmpezo) {
			int contador = 0;
			auto it = jugadores.begin();
			while (it != jugadores.end()) {
				Jugador* unJugador = *it;
				if (unJugador->estamuerto())
					contador++;
				it++;
			}
		if(contador == 1)			//configuracion.getCantidadMaximaJugadores()
				this->gameOver(false);
		} else {
			if(jugadores.size() == 1)   //configuracion.getCantidadMaximaJugadores()
				elJuegoEmpezo = true;
		}

		if (bossEliminado) {
			elJuegoEmpezo = false;
			bossEliminado = false;
			pasarDeNivel();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));				//lo duermo para q no consuma tanto recurso
	}
}

void Juego::pasarDeNivel() {
	if (nivel < 2) {
		nivel++;
		auto it = jugadores.begin();
		while (it != jugadores.end()) {
			Jugador* unJugador = *it;
			unJugador->getPos().x = 0;
			it++;
		}
	}
}

void Juego::updateWorld() {
	std::thread t_detenerEnemigoAnterior;
	Enemigo* enemigoSpawneado;
	while (!detenido) {
		if (elJuegoEmpezo) {
			srand(time(NULL));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 * ((rand() % 7) + 1)));
			if ((BossFinal != NULL || minPosXJugador < 5500)) {
				if (contadorEnemigosSpawneados < 6) {
					enemigoSpawneado = spawnEnemigo(tiempo());
				}
			}
			else {
				enemigoSpawneado = spawnBoss(tiempo());
				break;
			}

			for (auto it = proyectilesAliados.begin(); it != proyectilesAliados.end();) {
				Proyectil* unProyectil = *it;
				if (((unProyectil->getPos().x < escenario.getOffsetVista()) || unProyectil->getPos().x > (escenario.getOffsetVista() + escenario.getAnchoVista())) || (
					((unProyectil->getPos().y < 0) || unProyectil->getPos().y > configuracion.getTamanioVentana().y))) {
					delete unProyectil;
					proyectilesAliados.erase(it);
				}
				else
					it++;
			}

			for (auto it = proyectilesEnemigos.begin(); it != proyectilesEnemigos.end();) {
				Proyectil* unProyectil = *it;
				if (((unProyectil->getPos().x < escenario.getOffsetVista()) || unProyectil->getPos().x > (escenario.getOffsetVista() + escenario.getAnchoVista())) || (
					((unProyectil->getPos().y < 0) || unProyectil->getPos().y > configuracion.getTamanioVentana().y))) {
					delete unProyectil;
					proyectilesEnemigos.erase(it);
				}
				else
					it++;
			}
		}
	}
}

void Juego::detener() {
	if (!detenido) {
		detenido = true;
		t_updateWorld.join();
		debug("Thread updateWorld termino");
		t_updateIA.join();
		debug("Thread updateIA termino");
		t_chequearGameOverYComienzoDeJuego.join();
		debug("Thread chequeargameoverycomeinzodejuego termino");
	}
}

void Juego::agregarProyectilAliado(Proyectil * nuevoProyectil) {
	proyectilesAliados.push_back(nuevoProyectil);
}

void Juego::agregarProyectilEnemigo(Proyectil * nuevoProyectil) {
	proyectilesEnemigos.push_back(nuevoProyectil);
}

unsigned int Juego::getCantdadJugadores() {
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
	jugadoresEnJuego.push_back(jugador);
	if (!iniciado && jugadores.size() >= configuracion.getCantidadMinimaJugadores()) { //config
		iniciado = true;
		for (auto it = jugadores.begin(); it != jugadores.end(); it++)
			(*it)->setConectado(true);
	}
	cambios = true;
	lock.unlock();

	info("Jugador '" + nombre + "' creado");

	return jugador;
}

Enemigo* Juego::spawnBoss(micros tiempoCreacion) {
	Enemigo* nuevoEnemigo = NULL;
	int nivel = 0;
	if (nivel == 0) { //Se crea el Boss HI-DO
		nuevoEnemigo = new Boss0(++contador, configuracion);
		nuevoEnemigo->getTamanio().x = 200;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 250;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->getPos().y = 300;
		nuevoEnemigo->setTipo(Tipo::Boss1);
		lock.lock();
		nuevoEnemigo->setDistanciaPiso(escenario.getAnchoVista());
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		enemigos.push_back(nuevoEnemigo);
		contadorEnemigosSpawneados++;

		lock.unlock();
		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss HI-DO creado");
	}
	else if (nivel == 1 && (BossFinal == NULL)) { //Se crea el Boss AirbusterRiberts
		nuevoEnemigo = new Boss1(++contador, configuracion);
		nuevoEnemigo->getTamanio().x = 400;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 250;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->getPos().y = 300;
		nuevoEnemigo->setTipo(Tipo::Boss2);
		lock.lock();
		nuevoEnemigo->setDistanciaPiso(escenario.getAnchoVista());
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		enemigos.push_back(nuevoEnemigo);
		contadorEnemigosSpawneados++;

		lock.unlock();

		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss creado");
	}
	else if (nivel == 2 && (BossFinal == NULL)) { //Se crea el Boss TANI OH
		nuevoEnemigo = new Boss2(++contador, configuracion);
		nuevoEnemigo->getTamanio().x = 400;				//configurar para enemigo 
		nuevoEnemigo->getTamanio().y = 400;				//configurar para enemigo
		nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
		nuevoEnemigo->getPos().y = 150;
		nuevoEnemigo->setTipo(Tipo::Boss3);
		lock.lock();
		nuevoEnemigo->setDistanciaPiso(escenario.getAnchoVista());
		nuevoEnemigo->caminar(Direccion::IZQUIERDA);
		enemigos.push_back(nuevoEnemigo);
		contadorEnemigosSpawneados++;

		lock.unlock();

		if (BossFinal == NULL)
			BossFinal = nuevoEnemigo;

		cambios = true;

		info("Boss creado");
	}
	return nuevoEnemigo;
}


Enemigo* Juego::spawnEnemigo(micros tiempoCreacion) {
	Enemigo* nuevoEnemigo = new Enemigo(++contador, configuracion);

	nuevoEnemigo->getTamanio().x = configuracion.getTamanioJugador().x;				//configurar para enemigo 
	nuevoEnemigo->getTamanio().y = configuracion.getTamanioJugador().y;				//configurar para enemigo
	nuevoEnemigo->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista();
	nuevoEnemigo->setTiempoCreacion(tiempoCreacion);
	nuevoEnemigo->setTipo(Tipo::Enemigo);
	nuevoEnemigo->setDistanciaPiso(-nuevoEnemigo->getTamanio().y - nuevoEnemigo->getPos().y);
	lock.lock();
	nuevoEnemigo->caminar(Direccion::IZQUIERDA);
	enemigos.push_back(nuevoEnemigo);
	contadorEnemigosSpawneados++;

	lock.unlock();

	cambios = true;

	info("Enemigo creado");


	return nuevoEnemigo;
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estado;
	if (elJuegoEmpezo) {
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
					delete unPickUp;
					pickups.erase(it4);
					if (unJugador->getKillAll()) {
						auto it2 = enemigos.begin();
						while (it2 != enemigos.end()) {
							Enemigo* obj = *it2;
							delete (*it2);
							efectos.push_back(new EnemigoMuriendo(++contador, obj->getPos(), obj->getTamanio()));
							enemigos.erase(it2);;
						}
					}
				}
				it4++;
			}
			it++;
		}

		if (!enemigos.empty()) {
			minX = ultimoMinX;
		}

		ultimoMinX = minX;

		auto it2 = enemigos.begin();
		while (it2 != enemigos.end()) {
			Enemigo* enemigo = *it2;
			cambios |= enemigo->tieneCambios(plataformas);
			it2++;
		}

		for (auto it3 = proyectilesAliados.begin(); it3 != proyectilesAliados.end();) {
			Proyectil* unProyectil = *it3;
			if (unProyectil != NULL) {
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
							if ((unEnemigo->getTipo() == Tipo::Boss1) || (unEnemigo->getTipo() == Tipo::Boss2) || (unEnemigo->getTipo() == Tipo::Boss3)) {
								bossEliminado = true;
								contadorEnemigosSpawneados--; //elimino del total global
							} else {
								contadorEnemigosSpawneados--; //elimino del total global
								PickUp* nuevoPickup = unEnemigo->spawnPickUp();
								if (nuevoPickup != NULL)
									pickups.push_back(nuevoPickup);
							}
								it = jugadores.begin();
								bool encontrado = false;
								while ((it != jugadores.end()) && !encontrado) {
									Jugador* jugadorQueEliminoAlEnemigo = *it;
									if (unProyectil->getIdTirador() == jugadorQueEliminoAlEnemigo->getId())
										jugadorQueEliminoAlEnemigo->recibirPuntos(unEnemigo->getPuntos());
									it++;
								}
							efectos.push_back(new EnemigoMuriendo(++contador, unEnemigo->getPos(), unEnemigo->getTamanio()));
							delete unEnemigo;
							enemigos.erase(it2);
						}
					}
					it2++;
				}

				if (colisionan || !unProyectil->esVisible()) {
					efectos.push_back(new ImpactoBala(++contador, unProyectil));
					delete unProyectil;
					it3 = proyectilesAliados.erase(it3);
				}
				else
					it3++;
			}
		}

		for (auto it6 = proyectilesEnemigos.begin(); it6 != proyectilesEnemigos.end();) {
			Proyectil* unProyectil = *it6;
			if (unProyectil != NULL) {
				cambios |= unProyectil->tieneCambios(plataformas);
				bool colisionan = false;
				for (auto it = jugadores.begin(); (it != jugadores.end()) && !colisionan;) {
					Jugador *unJugador = *it;
					if (((unProyectil->getPos().x + unProyectil->getTamanio().x) < (unJugador->getPos().x)) || (unProyectil->getPos().x > unJugador->getPos().x + unJugador->getTamanio().x)) {
						colisionan = false;
					}
					else if (((unProyectil->getPos().y + unProyectil->getTamanio().y) < (unJugador->getPos().y)) || (unProyectil->getPos().y > unJugador->getPos().y + unJugador->getTamanio().y)) {
						colisionan = false;
					}
					else { colisionan = true; }
					bool estaMuerto = false;
					if (colisionan) {
						estaMuerto = unJugador->recibirDanio(unProyectil->getDanio());
						if (estaMuerto && modoDeJuego != MODO_DIOS) {
							unJugador->setEstaMuerto(true);
							jugadores.erase(it);
						}
						else
							it++;
					}
					else {
						it++;
					}
				}
				if (colisionan || !unProyectil->esVisible()) {
					delete unProyectil;
					it6 = proyectilesEnemigos.erase(it6);
				}
				else
					it6++;
			}

		}

		//Actualizar el offset si es necesario
		if (minX > escenario.getOffsetVista() + escenario.getAnchoVista() / 2) {
			cambios = true;
			int offset = std::min<int>(minX - escenario.getAnchoVista() / 2, escenario.getOffsetVista() + maxOffsetDelta);
			offset = std::min<int>(offset, escenario.getLongitud());
			escenario.setOffsetVista(offset);
		}

		//Actualizar posiciones de jugadores para que no se salgan de pantalla
		it = jugadores.begin();
		while (it != jugadores.end()) {
			Jugador* obj = *it;

			if ((obj->getPos().x + obj->getTamanio().x) > (escenario.getOffsetVista() + escenario.getAnchoVista())) {
				cambios = true;
				obj->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista() - obj->getTamanio().x;
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
				pos.y = escenario.getNivelPiso() + obj->getDistanciaPiso() - obj->getPos().y;
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
				estadoObj.setTipo(Tipo::BalaEnemigo);
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

		std::vector<InfoJugador> hudInfo;

		auto jugadorIt = jugadores.begin();
		while (jugadorIt != jugadores.end()) {
			Jugador* jugador = *jugadorIt;
			InfoJugador info;

			info.arma = jugador->getArma()->getTipo();
			info.balas = jugador->getArma()->getCantidad();
			setCharArray(jugador->getNombre(), info.nombre);
			info.puntos = jugador->getPuntos();
			info.energia = jugador->getEnergia();
			hudInfo.push_back(info);

			jugadorIt++;
		}


		rv = cambios;
		cambios = false;
		lock.unlock();

		if (rv) {
			bytes.put(nivel);
			bytes.put(escenario.getOffsetVista());
			bytes.putAll(estado);
			bytes.putAll(hudInfo);
		}
		return rv;
	} else {
		//devolver pantalla carga
		return false;
	}
}
