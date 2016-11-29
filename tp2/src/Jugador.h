/*
 * Jugador.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include "Logger.h"
#include "Config.h"
#include "Objeto.h"

enum class Direccion { IZQUIERDA, DERECHA };

class Jugador : public Objeto {
public:
	Jugador(int id, std::string nombre, Config& configuracion);
	~Jugador();

	void caminar(Direccion direccion);
	void detenerse();
	void saltar();

	void setConectado(bool conectado);
	std::string getNombre();

	virtual bool tieneCambios();

private:
	bool actualizar();
	bool killAll;
	Config& configuracion;
	bool cambios;
	std::string nombre;
	double velocCaminar, velocSaltoX, velocSaltoY;
	micros tiempoSalto;
	micros tiempoCaminando;
	std::mutex mutex;
};

#endif /* JUGADOR_H_ */
