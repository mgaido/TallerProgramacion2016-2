#include "GunC.h"

#include "Juego.h"

double velocidad = 0.0005;

GunC::GunC(int id) : Proyectil(id) {
	this->danio = 150;
	this->puntosOtorgados = 100;
	this->tipo = Tipo::GunC;
	this->estado = Estado::Normal;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->tiempoCreacion = tiempo();
}

bool GunC::tieneCambios(Juego* juego) {
	if (((tiempo() - tiempoCreacion) / 1000) > 150) {
		Punto posDestino = posEnemigoMasCercano(juego);

		double distX = posDestino.x - pos.x;
		double distY = posDestino.y - pos.y;

		double dist = distancia(posDestino, pos);

		if (dist != 0) {
			double parteX = distX / dist;
			double parteY = distY / dist;

			//suavizar
			double x, y;
			x = velocidadProyectilX / velocidad;
			y = velocidadProyectilY / velocidad;

			double cosAngle = x * parteX + y * parteY;
			double angle = acos(cosAngle);

			if (angle > 0.35) {
				cosAngle = cos(0.35);
				double sinAngle = sin(0.35);
				parteX = x * cosAngle - y * sinAngle;
				parteY = x * sinAngle + y * cosAngle;
			}
			velocidadProyectilX = velocidad * parteX;
			velocidadProyectilY = velocidad * parteY;
		}
	}
	return Proyectil::tieneCambios(juego);
}


Punto GunC::posEnemigoMasCercano(Juego* juego) {

	auto it = juego->getObjetos().begin();

	Punto posCercana;
	posCercana.x = pos.x + 10000 * (orientacion ? -1 : 1);
	posCercana.y = pos.y;
	double dist = distancia(pos, posCercana);

	while (it != juego->getObjetos().end()) {
		Objeto* obj = *it;
		if (esEnemigo(obj->getTipo())) {
			double dist2 = distancia(pos, obj->getPos());
			if (dist2 < dist) {
				dist = dist2;
				posCercana = obj->getPos();
				posCercana.x += obj->getTamanio().x / 2;
				posCercana.y += obj->getTamanio().y / 2;
			}
		}
		it++;
	}

	return posCercana;
}

double GunC::distancia(Punto pos1, Punto pos2) {
	double distanciaX = 0;
	double distanciaY = 0;

	distanciaX = pos1.x - pos2.x;
	distanciaY = pos1.y - pos2.y;

	return sqrt(distanciaX*distanciaX + distanciaY*distanciaY);
}

void GunC::setOrientacionX(bool orientacion) {
	velocidadProyectilX = orientacion ? -velocidad : velocidad;
}


EstadoObj GunC::getEstadoObj(Escenario& escenario) {
	EstadoObj estado = Proyectil::getEstadoObj(escenario);

	double angle = acos(velocidadProyectilX / velocidad) * 180 / PI;

	estado.setRotacion(angle);
	//std::cout << angle << std::endl;

	return estado;
}
