/*
 * Equipo.h
 *
 *  Created on: 13 dic. 2016
 *      Author: Rod
 */

#ifndef EQUIPO_H_
#define EQUIPO_H_

#include "Utils.h"
#include "Bytes.h"
#include "Puntaje.h"

class Equipo : public Serializable{
public:
	Equipo();
	~Equipo();

	bool tieneJugador(std::string nombre);
	void agregarJugador(std::string nombre);
	int cantidadDeJugadores();
	Puntaje& getPuntaje(std::string nombre);
	HashMap<std::string, Puntaje>& getPuntajes();

	virtual void toBytes(Bytes &bytes);
	virtual void fromBytes(Bytes &bytes);


private:
	HashMap<std::string, Puntaje> puntajes;
};

#endif /* EQUIPO_H_ */
