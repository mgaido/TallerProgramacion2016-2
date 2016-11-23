#include "Personaje.h"
#include "GunH.h"


Personaje::Personaje(int id, Config & _configuracion) : Objeto(id), configuracion(_configuracion) {
	energia = 1000;
	arma =  new GunH(46);  //agregar id 
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

void Personaje::cambiarArma(Proyectil *armaNueva) {
	arma = armaNueva;
}

void Personaje::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);

	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -configuracion.getVelocidadX();
	else
		velocCaminar = configuracion.getVelocidadX();

	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}

void Personaje::detenerse() {
	std::unique_lock<std::mutex> lock(mutex);
	actualizar();
	velocCaminar = 0;
	tiempoCaminando = 0;
}

void Personaje::saltar() {
	std::unique_lock<std::mutex> lock(mutex);

	if (tiempoSalto == 0) {
		velocSaltoY = configuracion.getVelocidadY();
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
}

bool Personaje::tieneCambios() {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar();
}

bool Personaje::actualizar() {
	if (estado != Estado::Desconectado) {

		micros t = 0;
		double vx = 0;

		if (tiempoSalto > 0) {
			vx = velocSaltoX;
			t = tiempo() - tiempoSalto;
			velocSaltoY -= t*configuracion.getGravedad();
			tiempoSalto += t;
		}
		else if (tiempoCaminando > 0) {
			vx = velocCaminar;
			t = tiempo() - tiempoCaminando;
			tiempoCaminando += t;
		}

		cambios |= t != 0;

		if (vx != 0)
			orientacion = vx < 0;

		pos.x += (int)round(vx*t);
		pos.y += (int)round(velocSaltoY*t);

		if (tiempoSalto > 0 && velocSaltoY < 0 && pos.y <= 0) {
			pos.y = 0;
			velocSaltoX = 0;
			velocSaltoY = 0;
			if (velocCaminar != 0)
				tiempoCaminando = tiempoSalto;
			tiempoSalto = 0;
		}

		if (t > 0 && pos.x < 0)
			pos.x = 0;

		Estado estado;
		if (tiempoSalto > 0)
			estado = Estado::Saltando;
		else if (tiempoCaminando > 0)
			estado = Estado::Caminando;
		else
			estado = Estado::Quieto;

		if (estado != this->estado) {
			this->estado = estado;
			cambios = true;
			frame = 0;
		}
	}

	bool rv = cambios;
	cambios = false;
	return rv;
}
