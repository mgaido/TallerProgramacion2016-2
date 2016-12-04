#pragma once
#ifndef PROYTECTIL_H_
#define PROYECTIL_H_

#include "Objeto.h"
#include "Plataforma.h"

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
	void setOrientacion(bool nuevaOrientacion);
	int getDanio();
	bool esVisible();
	int getIdTirador();
	void setVelocidadY(double velocidadProyectilY);
private:

protected:
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
