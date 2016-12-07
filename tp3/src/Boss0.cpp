#include "Boss0.h"
#include "Juego.h"

Boss0::Boss0(int id, Config & _configuracion) : Boss(id, _configuracion) {
	tamanio.x = 230;
	tamanio.y = 250;
	pos.y = configuracion.getTamanioVentana().y - tamanio.y * 1.2;

	energia = 2000;
	tipo = Tipo::Boss1;

	arma.setTipo(Tipo::Misil);

	tiempoUltimaBomba = tiempo();
	bombas = 0;
}

Proyectil* Boss0::disparar(Juego* juego) {

	bool orietacionAux = orientacion;

	if (arma.getTipo() != Tipo::Bomba) {
		int millis = (tiempo() - tiempoUltimaBomba) / 1000;

		if (millis > TIEMPO_ENTRE_BOMBAS) {
			bombas = 0;
			arma.setTipo(Tipo::Bomba);
		} else
			orientacion = (rand() % 2) == 0;
	}

	Proyectil* proyectil = Boss::disparar(juego);

	if (proyectil != nullptr && proyectil->getTipo() == Tipo::Bomba) {
		bombas++;
		if (bombas == BOMBAS_POR_VEZ) {
			std::cout << "Listo Bombas" << std::endl;
			tiempoUltimaBomba = tiempo();
			arma.setTipo(Tipo::Misil);
		}
	}

	orientacion = orietacionAux;

	return proyectil;
}

