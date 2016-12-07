#include "Boss1.h"
#include "Juego.h"
#include "EnemigoFuerte.h"

Boss1::Boss1(int id, Config & _configuracion) : Boss(id, _configuracion) {
	tamanio.x = 300;
	tamanio.y = 180;
	pos.y = configuracion.getTamanioVentana().y - tamanio.y * 1.25;

	energia = 2000;
	tipo = Tipo::Boss2;

	arma.setTipo(Tipo::Void);
	factorySeteada = false;
}

bool Boss1::tieneCambios(Juego* juego) {
	if (! factorySeteada) {
		juego->setEnemigoFactory(new EnemigoFactory(6, -1, [&](Juego* juego){ return new EnemigoFuerte(++contador, configuracion, this); }));
		factorySeteada = true;
	}

	bool cambios = Boss::tieneCambios(juego);
	cambios |= juego->getEnemigoFactory()->spawnEnemigo(juego);

	return cambios;
}
