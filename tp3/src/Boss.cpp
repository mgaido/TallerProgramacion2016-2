#include "Boss.h"


Boss::Boss(int id, Config & _configuracion) : Enemigo(id, _configuracion) {
	distanciaRecorrida = 0;
	this->energia = 10000;
}

void Boss::comportamiento(micros tiempoActual, std::vector<Proyectil*>* proyectilesEnemigos, std::vector<Enemigo*>* enemigos)
{
}

int modulo(int numero) {
	if (numero < 0)
		return -numero;
	else
		return numero;
}

bool Boss::tieneCambios(std::vector<Plataforma*>& plataformas){
	std::unique_lock<std::mutex> lock(mutex);
	int posXAntigua = pos.x;
	bool valor =  actualizar(plataformas);
	int posXNueva = pos.x;
	distanciaRecorrida += modulo((posXNueva - posXAntigua));
	return valor;
}

bool Boss::actualizar(std::vector<Plataforma*>& plataformas){
	micros t = 0;
	double vx = 0;

	 if (tiempoCaminando > 0) {
		vx = velocCaminar;
		t = tiempo() - tiempoCaminando;
		tiempoCaminando += t;
	}

	cambios |= t != 0;

	if (vx != 0)
		orientacion = vx < 0;


	pos.x = pos.x + (int)round(vx*t);

	if (t > 0 && pos.x < 0)
		pos.x = 0;

	if (tiempoCaminando > 0)
		estado = Estado::Caminando;
	else
		estado = Estado::Quieto;

	if (estado != this->estado) {
		this->estado = estado;
		cambios = true;
		frame = 0;
	}


	bool rv = cambios;
	cambios = false;
	return rv;
}


