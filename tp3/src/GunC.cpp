#include "GunC.h"

#include "Juego.h"

double velocidad = 0.0005;

double maxAngle = 0.157;
double cosMaxAngle = cos(maxAngle);
double senMaxAngle = sin(maxAngle);

GunC::GunC(int id) : Proyectil(id) {
	this->danio = 150;
	this->puntosOtorgados = 100;
	this->tipo = Tipo::GunC;
	this->estado = Estado::Normal;
	this->tamanio.x = 30;			//ver si este tamanio esta bien 
	this->tamanio.y = 30;
	this->orientacion = false;
	this->tiempoCreacion = tiempo();
	this->cooldown = 500;

	_angle = 0;
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

			double anguloActualHoriz = acos(x) * (y < 0 ? -1 : 1);
			double anguloDestHoriz = acos(parteX) * (parteY < 0 ? -1 : 1);
			double anguloEntreVectores = abs(anguloActualHoriz - anguloDestHoriz);

			if (anguloDestHoriz < -(PI/2))
				anguloDestHoriz += 2*PI;

			if (anguloEntreVectores > maxAngle) {
				int signo = 1;
				if (anguloActualHoriz < 0 && anguloDestHoriz < 0 && anguloDestHoriz < anguloActualHoriz)
					signo = -1;
				if (anguloActualHoriz >= 0 && (anguloDestHoriz < 0 || anguloDestHoriz < anguloActualHoriz))
					signo = -1;

				parteX = x * cosMaxAngle - y * senMaxAngle * signo;
				parteY = x * senMaxAngle * signo + y * cosMaxAngle;
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

void GunC::setOrientacionX(bool orientacion) {
	velocidadProyectilX = orientacion ? -velocidad : velocidad;
}

EstadoObj GunC::getEstadoObj(Escenario& escenario) {
	EstadoObj estado = Proyectil::getEstadoObj(escenario);

	double angle = acos(velocidadProyectilX / velocidad) * 180 / PI;
	if (velocidadProyectilY > 0)
		angle *= -1;

	estado.setRotacion(angle);

	return estado;
}
