#include "Boss2.h"
#include "Juego.h"

Boss2::Boss2(int id, Config & _configuracion) : Boss(id, _configuracion) {
	velocidadX = velocidadX / 2;

	tamanio.x = 250;
	tamanio.y = 175;
	pos.y = 0;

	energia = 1000;
	tipo = Tipo::Boss3;
}

bool Boss2::tieneCambios(Juego* juego) {

	int disparoLaser = rand() % 100;

	if (disparoLaser > 75)
		arma.setTipo(Tipo::Laser);
	else
		arma.setTipo(Tipo::Torreta);

	return Boss::tieneCambios(juego);
}
