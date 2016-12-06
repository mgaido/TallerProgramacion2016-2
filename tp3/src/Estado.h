/*
 * Actualizacion.h
 *
 *  Created on: Oct 4, 2016
 *      Author: rodrigo
 */

#ifndef ESTADO_H_
#define ESTADO_H_

#include "Objeto.h"

class EstadoObj {
public:
	EstadoObj() {
		nombre[0] = '\0';
		frame = 0;
	}

	inline Estado getEstado() const {
		return estado;
	}

	inline void setEstado(Estado estado) {
		this->estado = estado;
	}

	inline int getId() const {
		return id;
	}

	inline void setId(int id) {
		this->id = id;
	}

	inline Punto getPos() const {
		return pos;
	}

	inline void setPos(Punto pos) {
		this->pos = pos;
	}

	inline Punto getTamanio() const {
		return tamanio;
	}

	inline void setTamanio(Punto tamanio) {
		this->tamanio = tamanio;
	}

	inline Tipo getTipo() const {
		return tipo;
	}

	inline void setTipo(Tipo tipo) {
		this->tipo = tipo;
	}

	std::string getNombre() const {
		return std::string(nombre.data());
	}

	void setNombre(std::string nombre) {
		nombre.copy(this->nombre.data(), this->nombre.size());
		this->nombre[std::min<int>(nombre.size(), this->nombre.size()-1)] = '\0';
	}

	inline int getFrame() const {
		return frame;
	}

	inline void setFrame(int frame) {
		this->frame = frame;
	}

	bool getOrientacion() {
		return orientacion;
	}

	void setOrientacion(bool orientacion) {
		this->orientacion = orientacion;
	}

	inline std::string toString() {
		std::string estado;
			switch (getEstado()){
			case Estado::Caminando:
				estado = "caminando";
				break;
			case Estado::Quieto:
				estado = "quieto";
				break;
			case Estado::Saltando:
				estado = "saltando";
				break;
			case Estado::Desconectado:
				estado = "desconectado";
				break;
			default:
				estado = "???";
			}

			std::string tipo;
			switch (getTipo()) {
			case Tipo::GunH:
				tipo = "GunH";
				break;
			case Tipo::GunC:
				tipo = "GunC";
				break;
			case Tipo::GunS:
				tipo = "GunS";
				break;
			case Tipo::GunR:
				tipo = "GunR";
				break;
			case Tipo::GunF:
				tipo = "GunF";
				break;
			case Tipo::Jugador:
				tipo = "Jugador";
				break;
			case Tipo::Enemigo:
				tipo = "Enemigo";
				break;
			case Tipo::Boss1:
				tipo = "Boss1";
				break;
			case Tipo::Boss2:
				tipo = "Boss2";
				break;
			case Tipo::Boss3:
				tipo = "Boss3";
				break;
			case Tipo::BonusVida:
				tipo = "BonusVida";
				break;
			case Tipo::BonusKill:
				tipo = "BonusKill";
				break;
			case Tipo::BonusArma:
				tipo = "BonusArma";
				break;
			case Tipo::ImpactoH:
				tipo = "ImpactoH";
				break;
			case Tipo::ImpactoS:
				tipo = "ImpactoS";
				break;
			case Tipo::ImpactoF:
				tipo = "ImpactoF";
				break;
			case Tipo::ImpactoExp:
				tipo = "ImpactoExp";
				break;
			case Tipo::EnemigoMuriendo:
				tipo = "EnemigoMuriendo";
				break;
			default:
				tipo = "????";
			}

			std::stringstream ss;
			ss << tipo << " " << getId() << " " << getNombre() << " " << estado << " en (" << getPos().x << ", " << getPos().y << ")" << " Tamanio (" << getTamanio().x << ", " << getTamanio().y << ")";
			return ss.str();
	}

private:
	int id;
	Punto pos;
	Punto tamanio;
	Estado estado;
	Tipo tipo;
	std::array<char, 100> nombre;
	int frame;
	bool orientacion;
};


class InfoJugador {
public:
	std::array<char, 100> nombre;
	int energia;
	int puntos;
	Tipo arma;
	int balas;
};

#endif /* ESTADO_H_ */
