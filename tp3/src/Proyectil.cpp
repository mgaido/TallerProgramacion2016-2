#include "Proyectil.h"
#include "Juego.h"
#include "Personaje.h"
#include "Jugador.h"
#include "Enemigo.h"
#include "ImpactoBala.h"

Proyectil::Proyectil(int id) : Objeto(id) {
	orientacionEjeY = DISPARAR_NEUTRO;
	disparadorSaltando = false;
	velocidadProyectilX = 0;
	velocidadProyectilY = 0;
	tiempoEnMovimiento = tiempo();
	tirador = nullptr;
	idTirador = 0;
	tipoTirador = Tipo::Void;
	puntosOtorgados = 0;
	danio = 0;
	this->estado = Estado::ProyectilEnMovimiento;
}

Proyectil::~Proyectil() {}

void Proyectil::setTirador(Personaje* tirador) {
	this->idTirador = tirador->getId();
	this->tipoTirador = tirador->getTipo();
	if (tipoTirador == Tipo::Jugador)
		this->tirador = (Jugador*) tirador;
}

bool Proyectil::tieneCambios(Juego* juego) {
	micros  t = tiempo() - tiempoEnMovimiento;
	tiempoEnMovimiento += t;

	pos.x  = pos.x + (int)round(velocidadProyectilX*t);
	pos.y = pos.y + (int)round(velocidadProyectilY*t);

	bool colisionan = false;

	auto it = juego->getPlataformas().begin();
	while (it != juego->getPlataformas().end() && !colisionan) {
		colisionan = colisionaCon(*it);
		puntosOtorgados = 0;
		it++;
	}

	if (! colisionan) {
		auto it2 = juego->getObjetos().begin();
		while (it2 != juego->getObjetos().end() && !colisionan) {
			Objeto* objeto = *it2;

			if (tipoTirador == Tipo::Jugador && esEnemigo(objeto->getTipo())) {
				colisionan = colisionaCon(objeto);
				if (colisionan) {
					Enemigo* unEnemigo = (Enemigo*) objeto;
					tirador->recibirPuntos(puntosOtorgados);
					bool estaMuerto = unEnemigo->recibirDanio(danio);
					if (estaMuerto) {
						PickUp* nuevoPickup = unEnemigo->spawnPickUp();
						if (nuevoPickup != nullptr)
							juego->agregarObjeto(nuevoPickup);
					}
				}
			}

			if (esEnemigo(tipoTirador) && objeto->getTipo() == Tipo::Jugador) {
				colisionan = colisionaCon(objeto);
				if (colisionan) {
					Jugador* impactado = (Jugador*) objeto;
					impactado->recibirDanio(danio);

				}
			}

			it2++;
		}
	}

	visible = ! colisionan;
	return true;
}

int Proyectil::getPuntos() {
	return puntosOtorgados;
}

void Proyectil::setSiElDisparadorEstaSaltando(bool disparadorSaltando) {
	this->disparadorSaltando = disparadorSaltando;
}

void Proyectil::setOrientacionX(bool nuevaOrientacion) {
	orientacion = nuevaOrientacion;
	if (orientacion)
		velocidadProyectilX = -0.0007;
	else
		velocidadProyectilX = 0.0007;
}

void Proyectil::setOrientacionY(char nuevaOrientacion) {
	orientacionEjeY = nuevaOrientacion;
	if (orientacionEjeY == DISPARAR_ARRIBA) {
		velocidadProyectilY = 0.0007;
		velocidadProyectilX = 0;
		//estado = Estado::ProyectilHaciaArriba;
	}
	else if (orientacionEjeY == DISPARAR_ABAJO) {
		if (disparadorSaltando) {
			velocidadProyectilY = -0.0007;
			velocidadProyectilX = 0;
		//	estado = Estado::ProyectilHaciaAbajo;
		} else {
			velocidadProyectilY = 0;
			pos.y -= 40; //por estar agachado
		}
	} else
		velocidadProyectilY = 0;
}

int Proyectil::getDanio(){
	return danio;
}

void Proyectil::setVelocidadY(double velocidadProyectilY){
	this->velocidadProyectilY = velocidadProyectilY;
}

void Proyectil::setPos(Punto nuevaPosicion) {
	this->pos.x = nuevaPosicion.x;
	this->pos.y = nuevaPosicion.y + 120;
}

Efecto* Proyectil::efectoAlDesaparecer() {
	return new ImpactoBala(++contador, this);
}

EstadoObj Proyectil::getEstadoObj(Escenario& escenario) {
	EstadoObj estado = Objeto::getEstadoObj(escenario);

	estado.setOrientacion(false);

	if (velocidadProyectilX > 0 && velocidadProyectilY == 0)
		estado.setRotacion(0);

	if (velocidadProyectilX > 0 && velocidadProyectilY < 0)
		estado.setRotacion(45);

	if (velocidadProyectilX == 0 && velocidadProyectilY < 0)
		estado.setRotacion(90);

	if (velocidadProyectilX < 0 && velocidadProyectilY < 0)
		estado.setRotacion(135);

	if (velocidadProyectilX < 0 && velocidadProyectilY == 0)
		estado.setRotacion(180);

	if (velocidadProyectilX < 0 && velocidadProyectilY > 0)
		estado.setRotacion(225);

	if (velocidadProyectilX == 0 && velocidadProyectilY > 0)
		estado.setRotacion(270);

	if (velocidadProyectilX > 0 && velocidadProyectilY > 0)
		estado.setRotacion(315);


	return estado;
}

