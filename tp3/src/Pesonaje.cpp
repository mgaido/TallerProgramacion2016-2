#include "Personaje.h"
#include "GunH.h"


Personaje::Personaje() {
	energia = 1000;
	arma = GunH();
}

int Personaje::getEnergia() {
	return energia;
}

bool Personaje::recibirDanio(int danio) {
	bool muerto = false;
	if (energia > danio) {
		energia = energia - danio;
	} else {
		energia = 0;
		muerto = true;
	}
	return muerto;
}

void Personaje::cambiarArma(Proyectil armaNueva) {
	arma = armaNueva;
}