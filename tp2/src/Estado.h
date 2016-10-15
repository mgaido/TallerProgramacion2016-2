/*
 * Estado.h
 *
 *  Created on: Oct 4, 2016
 *      Author: rodrigo
 */

#ifndef ESTADO_H_
#define ESTADO_H_

#include "Objeto.h"

class EstadoObj {
public:
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
			case Tipo::Jugador:
				tipo = "Jugador";
				break;
			default:
				tipo = "????";
			}

			std::stringstream ss;
			ss << tipo << " " << getId() << " " << estado << " en (" << getPos().x << ", " << getPos().y << ")";
			return ss.str();
	}


private:
	int id;
	Punto pos;
	Punto tamanio;
	Estado estado;
	Tipo tipo;
};

#endif /* ESTADO_H_ */
