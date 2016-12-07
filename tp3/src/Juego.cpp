/*
 * Juego.cpp
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#include "Juego.h"
#include "Efecto.h"
#include "Enemigo.h"
#include "Jugador.h"

#include "Boss.h"
#include "Boss0.h"
#include "Boss1.h"
#include "Boss2.h"


int maxOffsetDelta;
int contador = 0;

Juego::Juego(Config& _configuracion) : configuracion(_configuracion) {
	estado = EstadoJuego::NoIniciado;
	BossFinal = nullptr;
	cambios = true;
	crearPlataformas();

	escenario = Escenario(configuracion.getLongitud(), configuracion.getTamanioVentana().x, configuracion.getTamanioVentana().y, configuracion.getNivelPiso());
	maxOffsetDelta = round(configuracion.getVelocidadX() * 2 * 1000000.0 / configuracion.getFrameRate());

	enemigoFactory = new EnemigoFactory(6, -1, [&](Juego* juego){ return new Enemigo(++contador, configuracion, escenario); });
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

Juego::~Juego() {
	auto it = objetos.begin();
	while (it != objetos.end()) {
		Objeto* obj = *it;
		if (obj->getTipo() != Tipo::Jugador)
			delete obj;
		objetos.erase(it);
	}    

	auto it2 = plataformas.begin();
	while (it2 != plataformas.end()) {
		Plataforma* obj = *it2;
		delete obj;
		plataformas.erase(it2);
	}

	auto it3 = jugadores.begin();
	while (it3 != jugadores.end()) {
		Jugador* obj = *it3;
		delete obj;
		jugadores.erase(it3);
	}

	if (enemigoFactory != nullptr)
		delete enemigoFactory;
}

std::vector<Objeto*>& Juego::getObjetos() {
	return objetos;
}

std::vector<Plataforma*>& Juego::getPlataformas() {
	return plataformas;
}

Config& Juego::getConfiguracion(){
	return configuracion;
}

Escenario& Juego::getEscenario(){
	return escenario;
}

void Juego::agregarObjeto(Objeto *objeto){
	nuevosLock.lock();
	nuevosObjetos.push_back(objeto);
	nuevosLock.unlock();
	cambios = true;
}

unsigned int Juego::getCantdadJugadores(){
	return jugadores.size();
}

bool Juego::estaIniciado() {
	return estado != EstadoJuego::NoIniciado;
}

Jugador* Juego::nuevoJugador(std::string nombre) {
	Jugador* jugador = new Jugador(++contador, nombre, configuracion);
	jugador->getTamanio().x = configuracion.getTamanioJugador().x;
	jugador->getTamanio().y = configuracion.getTamanioJugador().y;
	jugador->getPos().x = escenario.getOffsetVista();
	lock.lock();
	agregarObjeto(jugador);
	jugadores.push_back(jugador);
	if (estado == EstadoJuego::NoIniciado && getCantdadJugadores() >= configuracion.getCantidadMinimaJugadores()) { //config
		estado = EstadoJuego::EnJuego;
		for (auto it = objetos.begin(); it != objetos.end(); it++) {
			if ((*it)->getTipo() == Tipo::Jugador) {
				Jugador* jugador = (Jugador*) *it;
				jugador->setConectado(true);
			}
		}
	}
	cambios = true;
	lock.unlock();

	info("Jugador '" + nombre + "' creado");
	return jugador;
}

bool Juego::getEstado(Bytes& bytes) {
	bool rv = false;;
	std::vector<EstadoObj> estadoObjs;
	std::vector<InfoJugador> hudInfo;

	lock.lock();

	//Actualizar posiciones de objetos y encontrar el minimo en X de jugadores
	int minX = 0;

	for (auto it = objetos.begin(); it != objetos.end(); it++) {
		Objeto* objeto = *it;
		bool tieneCambios = objeto->tieneCambios(this);
		cambios |= tieneCambios;

		if (objeto->getTipo() == Tipo::Jugador) {
			if ((minX == 0 || objeto->getPos().x < minX) && objeto->getEstado() != Estado::Desconectado)
				minX = objeto->getPos().x + objeto->getTamanio().x / 2;
		}
	}

	//minX = std::max<int>(minX, 6800);

	//Actualizar el offset si es necesario
	if (minX > escenario.getOffsetVista() + escenario.getAnchoVista()/2) {
		cambios = true;
		int offset = std::min<int>(minX - escenario.getAnchoVista()/2, escenario.getOffsetVista() + maxOffsetDelta);
		offset = std::min<int>(offset, escenario.getLongitud());
		escenario.setOffsetVista(offset);
	}

	//Spawnear boss si se llego al final
	if (BossFinal == nullptr && escenario.getOffsetVista()>= escenario.getLongitud())
		spawnBoss();

	if (BossFinal == nullptr)
		cambios |= enemigoFactory->spawnEnemigo(this);

	//Segunda pasada
	auto it = objetos.begin();
	while (it != objetos.end()) {
		Objeto* objeto = *it;
		bool remover = false;
		Efecto* efecto = nullptr;


		if (! objeto->esVisible()) {
			remover = true;
			efecto = objeto->efectoAlDesaparecer();
		} else if (objeto->getPos().y < (escenario.getNivelPiso() - escenario.getAltoVista())|| objeto->getPos().y > escenario.getAltoVista()) {
			remover = true;
			efecto = objeto->efectoAlDesaparecer();
		} else if (objeto->getPos().x < escenario.getOffsetVista() - objeto->getTamanio().x || objeto->getPos().x > escenario.getOffsetVista() + escenario.getAnchoVista())
			remover = true;

		if (remover) {
			if (dynamic_cast<Enemigo*>(objeto))
				enemigoFactory->decrementarCantidadEnemigos();
			objetos.erase(it);
			if (objeto->getTipo() != Tipo::Jugador)
				delete objeto;
		} else {
			//Actualizar posiciones de jugadores para que no se salgan de pantalla
			if (objeto->getTipo() == Tipo::Jugador) {
				Jugador* jugador = (Jugador*) objeto;

				if ((jugador->getPos().x + jugador->getTamanio().x) > (escenario.getOffsetVista() + escenario.getAnchoVista())) {
					cambios = true;
					jugador->getPos().x = escenario.getOffsetVista() + escenario.getAnchoVista()- jugador->getTamanio().x;
				}

				if (jugador->getPos().x < escenario.getOffsetVista()) {
					cambios = true;
					jugador->getPos().x = escenario.getOffsetVista();
				}
			}
			it++;
		}

		if (efecto != nullptr)
			agregarObjeto(efecto);
	}

	nuevosLock.lock();
	it = nuevosObjetos.begin();
	while (it != nuevosObjetos.end()) {
		objetos.push_back(*it);
		nuevosObjetos.erase(it);
	}

	nuevosLock.unlock();

	//Generar estado para la vista
	if (cambios) {
		it = objetos.begin();
		while (it != objetos.end()) {
			Objeto* obj = *it;
			if (obj->esVisible()) {
				EstadoObj estadoObj = obj->getEstadoObj(escenario);
				estadoObjs.push_back(estadoObj);
			}
			it++;
		}

		auto itPlataformas = plataformas.begin();
		while (itPlataformas != plataformas.end()) {
			Objeto* obj = *itPlataformas;

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

				estadoObjs.push_back(estadoObj);
			}
			itPlataformas++;
		}

		int jugadoresVivos = jugadores.size();

		for (auto itJugadores = jugadores.begin(); itJugadores != jugadores.end(); itJugadores++) {
			Jugador* jugador = *itJugadores;

			InfoJugador info;
			info.arma = jugador->getArma().getTipo();
			info.balas = jugador->getArma().getBalas();
			setCharArray(jugador->getNombre(), info.nombre);
			info.puntos = jugador->getPuntos();
			info.energia = jugador->getEnergia();
			hudInfo.push_back(info);

			if (! jugador->esVisible())
				jugadoresVivos --;
		}

		if (estaIniciado() && jugadoresVivos == 0) {
			estado = EstadoJuego::Perdido;
			info("GAME OVER", true);
		}

		if (BossFinal != nullptr && ! BossFinal->esVisible()) {
			estado = EstadoJuego::Ganado;
			info("STAGE CLEAR", true);
		}
	}
	rv = cambios;
	cambios = false;
	lock.unlock();

	if (rv) {
		bytes.put(estado);
		bytes.put(escenario.getOffsetVista());
		bytes.putAll(estadoObjs);
		bytes.putAll(hudInfo);
	}
	return rv;
}

void Juego::spawnBoss() {
	int nivel = 0;
	Boss* boss = nullptr;

	if (nivel == 0 )
		boss = new Boss0(++contador, configuracion);

	if (nivel == 1 )
		boss = new Boss1(++contador, configuracion);

	if (nivel == 2 )
		boss = new Boss2(++contador, configuracion);

	if (boss != nullptr) {
		info("Boss creado", true);
		boss->getPos().x = escenario.getLongitud() + escenario.getAnchoVista();
		agregarObjeto(boss);
		BossFinal = boss;
		cambios = true;
	}
}

EnemigoFactory* Juego::getEnemigoFactory() {
	return enemigoFactory;
}

void Juego::setEnemigoFactory(EnemigoFactory* enemigoFactory) {
	if (this->enemigoFactory != nullptr)
		delete this->enemigoFactory;
	this->enemigoFactory = enemigoFactory;
}

bool Juego::estaPerdido() {
	return estado == EstadoJuego::Perdido;
}

bool Juego::estaGanado() {
	return estado == EstadoJuego::Ganado;
}

