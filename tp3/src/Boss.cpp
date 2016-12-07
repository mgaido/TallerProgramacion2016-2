#include "Boss.h"
#include "Juego.h"


Boss::Boss(int id, Config & _configuracion) : Personaje(id, _configuracion) {
	velocidadX = 0.0001;
	orientacion = true;
	tiempoMovimiento = tiempo();
	tiempoUltimoDisparo = tiempo();
	estado = Estado::Normal;
}

bool Boss::tieneCambios(Juego* juego) {
	micros t = tiempo() - tiempoMovimiento;
	pos.x += velocidadX * t * (orientacion ? -1 : 1);

	Escenario escenario = juego->getEscenario();

	int random = rand() % 100 + 15;

	if (orientacion && pos.x < escenario.getOffsetVista() + random) {
		orientacion = false;
		pos.x  = escenario.getOffsetVista() + random;
	} else if (! orientacion && pos.x > escenario.getOffsetVista() + escenario.getAnchoVista() - tamanio.x - random) {
		pos.x = escenario.getOffsetVista() + escenario.getAnchoVista() - tamanio.x - random;
		orientacion = true;
	}

	tiempoMovimiento += t;

	int millisUltimoDisparo = (tiempo() - tiempoUltimoDisparo) / 1000;
	int millis = (rand() % 20 + 5) * 100;

	if (millisUltimoDisparo > millis) {
		this->disparar(juego);
		tiempoUltimoDisparo = tiempo();
	}

	return true;
}
