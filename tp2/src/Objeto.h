/*
 * Objeto.h
 *
 *  Created on: 2 oct. 2016
 *      Author: Rod
 */

#ifndef OBJETO_H_
#define OBJETO_H_

#include "Utils.h"

enum class Tipo { Jugador };
enum class Estado { Quieto, Caminando, Saltando, Desconectado };


class Punto {
public:
	int x;
	int y;
	Punto(){
		x=0;
		y=0;
	}
};

class Escenario {
public:
	Escenario();
	Escenario(int longitud, int anchoVista, int nivelPiso);

	int getLongitud() const;
	int getAnchoVista() const;
	int getNivelPiso() const;
	int getOffsetVista() const;
	void setOffsetVista(int offsetVista);
	int getFrame();
private:
	int longitud;
	int anchoVista;
	int nivelPiso;
	int offsetVista;
};

class Objeto {
public:
	Objeto(int id);
	virtual ~Objeto();

	int getId();
	Punto& getPos();
	Punto& getTamanio();
	Estado getEstado();
	Tipo getTipo();
	int getFrame();

	virtual bool tieneCambios();


protected:
	int id;
	Punto pos;
	Punto tamanio;
	Estado estado;
	Tipo tipo;
	int frame;

};

#endif /* OBJETO_H_ */
