#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"
#include "Plataforma.h"

const char DISPARAR_ARRIBA = 0;
const char DISPARAR_ABAJO = 1;
const char DISPARAR_NEUTRO = 2;

class Proyectil : public Objeto {
public:
	Proyectil(int id, int idJugador);
	void recargar();
	int getPuntos();
	void setPos(Punto nuevaPosicion);
	virtual bool tieneCambios(std::vector<Plataforma*>& plataformas);
	Proyectil* disparar(); //Devuelve True si se pudo disparar, False si se quedo sin Balas.
	virtual Proyectil* dispararEspecial(Objeto * enemigoMasCercano);
	~Proyectil();
	virtual void trayectoria(std::vector<Plataforma*>& plataformas);
	void setOrientacionX(bool nuevaOrientacion);
	void setOrientacionY(char nuevaOrientacion);
	int getDanio();
	bool esVisible();
	int getIdTirador();
	void setVelocidadY(double velocidadProyectilY);
	void setSiElDisparadorEstaSaltando(bool saltando);
	void aumentarDanio(int aumento);
	int getCantidad();
	void setCantidad(int cantidad);
private:

protected:
	bool disparadorSaltando;
	char orientacionEjeY;
	int idTirador;
	std::mutex mutex;
	bool visible;
	virtual Proyectil* crearProyectil();
	int danio;
	int cantidad;
	int cantidadRepuesto;
	int puntosOtorgados;
	int tiempoUltimoDisparo;
	double velocidadProyectilX;
	double velocidadProyectilY;
	double tiempoEnMovimiento;
	bool cambios;
};

#endif /* PROYECTIL_H_ */
