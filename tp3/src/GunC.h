#pragma once
#ifndef GUNC_H_
#define GUNC_H_

#include "Proyectil.h"

//Misiles que siguen al Jugador
class GunC : public Proyectil {
public:
	GunC(int id, int idJugador);
	GunC(int id, int idJugador, Objeto* enemigoMasCercano);
	virtual Proyectil* dispararEspecial(Objeto *enemigoMasCercano);
	virtual void trayectoria(std::vector<Plataforma*>& plataformas);
	
private:
	int modulo(int numero);
	int mismoSignoQueEnemigoMasCercano(int velocidad, int pos);
protected:
	virtual Proyectil* crearProyectil();
	virtual Proyectil* crearProyectil(Objeto* enemigoMasCercano);
	Objeto* enemigoMasCercano;
};

#endif /* GUNC_H_ */
