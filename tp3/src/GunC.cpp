#include "GunC.h"

GunC::GunC(int id, int idJugador) : Proyectil(id, idJugador) {
	this->danio = 150;
	this->cantidad = 40;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 100;
	this->tipo = Tipo::GunC;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->frame = 0;
	this->enemigoMasCercano = NULL;
}

GunC::GunC(int id, int idJugador, Objeto * enemigoMasCercano) : Proyectil(id, idJugador) {
	this->danio = 150;
	this->cantidad = 40;
	this->cantidadRepuesto = 15;
	this->puntosOtorgados = 100;
	this->tipo = Tipo::GunC;
	this->estado = Estado::ProyectilEnMovimiento;
	this->tamanio.x = 50;			//ver si este tamanio esta bien 
	this->tamanio.y = 50;
	this->orientacion = false;
	this->frame = 0;
	this->enemigoMasCercano = enemigoMasCercano;
}

void GunC::trayectoria(std::vector<Plataforma*>& plataformas){
/*		micros t = 0;
		double velocidad = 0;

		if (tiempoEnMovimiento > 0) {
			velocidad = velocidadProyectilX;
			t = tiempo() - tiempoEnMovimiento;
			tiempoEnMovimiento += t;
		}

		cambios |= t != 0;
		int nuevaPosX;
		int nuevaPosY;
		try {
			velocidad = mismoSignoQueEnemigoMasCercano(velocidad, enemigoMasCercano->getPos().x);
			if (velocidad != 0)
				orientacion = velocidad < 0;

			nuevaPosX = pos.x + (int)round(velocidad*t);
			if (!(modulo(nuevaPosX) < modulo(enemigoMasCercano->getPos().x)))
				nuevaPosX = pos.x;

			nuevaPosY = pos.y + (int)round(velocidad*t);
			velocidad = mismoSignoQueEnemigoMasCercano(velocidad, enemigoMasCercano->getPos().y);   //creo q hay q mejorar un poco mas esto
			if (!(modulo(nuevaPosY) < modulo(enemigoMasCercano->getPos().y)))
				nuevaPosY = pos.y;
		} catch (...) {
			if (velocidad != 0)
				orientacion = velocidad < 0;
			nuevaPosX = pos.x + (int)round(velocidad*t);
			nuevaPosY = pos.y + (int)round(velocidad*t);
		}


		bool colisionan = false;
		auto it = plataformas.begin();
		while (it != plataformas.end() && !colisionan) {
			Plataforma* unaPlataforma = *it;
			if (((nuevaPosX + getTamanio().x) < (unaPlataforma->getPos().x)) || (nuevaPosX > unaPlataforma->getPos().x + unaPlataforma->getTamanio().x)) {
				colisionan = false;
			}
			else if (((nuevaPosY + getTamanio().y) < (unaPlataforma->getPos().y)) || (nuevaPosY > unaPlataforma->getPos().y + unaPlataforma->getTamanio().y)) {
				colisionan = false;
			}
			else { colisionan = true; }
			it++;
		}

		if (!colisionan) {
			pos.x = nuevaPosX;
			pos.y = nuevaPosY;
		} else
			visible = false;
			*/
}


int GunC::modulo(int numero){
	if (numero >= 0)
		return numero;
	else
		return -numero;
}

int GunC::mismoSignoQueEnemigoMasCercano(int velocidad, int pos){
	if (pos < 0)
		return -velocidad;
	else
		return velocidad;
}

Proyectil * GunC::crearProyectil()
{
	return new GunC(++contador, idTirador);
}

Proyectil* GunC::crearProyectil(Objeto *enemigoMasCercano) {
	return new GunC(++contador, idTirador, enemigoMasCercano);
}

Proyectil* GunC::dispararEspecial(Objeto* enemigoMasCercano) {
	Proyectil* nuevoProyectil = NULL;
	if (cantidad > 0) {
		cantidad = cantidad - 1;
		nuevoProyectil = crearProyectil(enemigoMasCercano);
	}
	return nuevoProyectil;
}
