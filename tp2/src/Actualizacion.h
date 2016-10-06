/*
 * Actualizacion.h
 *
 *  Created on: Oct 4, 2016
 *      Author: rodrigo
 */

#ifndef ACTUALIZACION_H_
#define ACTUALIZACION_H_

#include "Objeto.h"

class Actualizacion {
public:
	inline Estado getEstado() const {
		return estado;
	}

	inline void setEstado(Estado estado) {
		this->estado = estado;
	}

	inline Evento getEvento() const {
		return evento;
	}

	inline void setEvento(Evento evento) {
		this->evento = evento;
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

			std::string evento;

			switch (getEvento()) {
			case Evento::Agregar:
				evento = "Agregar";
				break;
			case Evento::Modificar:
				evento = "Modificar";
				break;
			case Evento::Eliminar:
				evento = "Eliminar";
				break;
			default:
				evento = "?????";
			}

			std::stringstream ss;
			ss << evento << " " << tipo << " " << getId() << " " << estado << " en (" << getPos().x << ", " << getPos().y << ")";
			return ss.str();
	}


private:
	int id;
	Punto pos;
	Punto tamanio;
	Estado estado;
	Evento evento;
	Tipo tipo;
};

#endif /* ACTUALIZACION_H_ */
