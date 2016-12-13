
#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Soldado.h"
#include "PickUp.h"
#include "Escenario.h"
 
class Enemigo : public Soldado {
public:
	Enemigo(int id, Config& _configuracion);
	Enemigo(int id, Config& _configuracion, Escenario& escenario);
	~Enemigo();

	virtual bool tieneCambios(Juego* juego);
	virtual bool comportamiento(Juego* juego);

	PickUp* spawnPickUp();

	void caminar(Direccion direcc);
	void saltar();
	void setTiempoCreacion(micros tiempoCreacion);

protected:
	micros tiempoCreacion;
	micros tiempoUltimoDisparo;

	Punto posJugadorMasCercano(Juego* juego);
};

#endif /* ENEMIGO_H_ */
