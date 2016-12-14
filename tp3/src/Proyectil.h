#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"

const char DISPARAR_ARRIBA = 0;
const char DISPARAR_ABAJO = 1;
const char DISPARAR_NEUTRO = 2;

class Personaje;
class Jugador;

class Proyectil : public Objeto {
public:
	Proyectil(int id);
	~Proyectil();

	virtual bool tieneCambios(Juego* juego);
	virtual Efecto* efectoAlDesaparecer();

	void setPos(Punto nuevaPosicion);
	void setTirador(Personaje* tirador);

	void setSiElDisparadorEstaSaltando(bool saltando);
	virtual void setOrientacionX(bool orientacion);
	virtual void setOrientacionY(char orientacion);
	virtual void setVelocidadY(double velocidadProyectilY);



	virtual EstadoObj getEstadoObj(Escenario& escenario);


	int getDanio();
	int getPuntos();
	int getCooldown();

	double getVelocidadProyectilY() const {
		return velocidadProyectilY;
	}

	void setVelocidadProyectilX(double velocidadProyectilX) {
		this->velocidadProyectilX = velocidadProyectilX;
	}

protected:
	int idTirador;
	Tipo tipoTirador;
	Jugador* tirador;

	char orientacionEjeY;
	bool disparadorSaltando;
	double velocidadProyectilX;
	double velocidadProyectilY;
	double tiempoEnMovimiento;

	int puntosOtorgados;
	int danio;
	int cooldown;
};

#endif /* PROYECTIL_H_ */
