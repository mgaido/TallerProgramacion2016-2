/*
 * Objeto.h
 *
 *  Created on: 2 oct. 2016
 *      Author: Rod
 */

#ifndef OBJETO_H_
#define OBJETO_H_

#include "Utils.h"
#include "Estado.h"
#include "Escenario.h"

class Juego;
class Efecto;

class Objeto {
public:
	Objeto(int id);
	virtual ~Objeto();

	int getId();
	Punto& getPos();
	Punto& getTamanio();
	Estado getEstado();
	Tipo getTipo();
	void setTipo(Tipo tipo);

	int getFrame();
	bool getOrientacion();

	virtual bool tieneCambios(Juego* juego);
	virtual bool esVisible();

	virtual Efecto* efectoAlDesaparecer();

	virtual EstadoObj getEstadoObj(Escenario& escenario);

	virtual std::string getNombre();

protected:
	bool colisionaCon(Objeto* otro);

	int id;
	Punto pos;
	Punto tamanio;
	Estado estado;
	Tipo tipo;
	int frame;
	bool orientacion;
	bool visible;
};

#endif /* OBJETO_H_ */
