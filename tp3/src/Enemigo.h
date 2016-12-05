
#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Personaje.h"
#include "PickUp.h"
#include <cstdlib>
 
class Enemigo : public Personaje {
public:
	Enemigo(int id, Config& _configuracion);
	~Enemigo();
	virtual bool esEnemigo();
	PickUp* spawnPickUp();
	//void caminar(Direccion direc);
	int getPuntos();
	int getDistanciaPiso();
	void setDistanciaPiso(int distancia);
	void caminar(Direccion direcc);
	void saltar();
	virtual Proyectil* disparar();
	virtual void comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos);
	void setTiempoCreacion(micros tiempoCreacion);
	void aumentarEnergia();
	void aumentarDanio();

protected:
	micros tiempoCreacion;
	micros tiempoUltimoDisparo;
	int aumentoDeDanio;
	int puntosQueDaAlMorir;
	int distanciaPiso;
};

#endif /* ENEMIGO_H_ */
