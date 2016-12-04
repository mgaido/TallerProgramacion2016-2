#include "Proyectil.h"

Proyectil::Proyectil(int id, int idJugador) : Objeto(id) {
	tiempoUltimoDisparo = 0;
	cambios = false;
	visible = true; 
	velocidadProyectilX = 0;
	velocidadProyectilY = 0;
	tiempoEnMovimiento = 0;
	idTirador = idJugador;
}

void Proyectil::recargar() {
	cantidad = cantidad + cantidadRepuesto;
}

int Proyectil::getPuntos() {
	return puntosOtorgados;
}

Proyectil* Proyectil::disparar() {
	Proyectil* nuevoProyectil = NULL;
	if (cantidad > 0) {
		cantidad = cantidad - 1;
		nuevoProyectil = crearProyectil();
	}
	return nuevoProyectil;
}

Proyectil::~Proyectil() {}

Proyectil* Proyectil::dispararEspecial(Objeto* enemigoMasCercano) {
	return NULL;
}

void Proyectil::setOrientacion(bool nuevaOrientacion) {
	orientacion = nuevaOrientacion;
	tiempoEnMovimiento = tiempo();
	if (orientacion)
		velocidadProyectilX = -0.0004;
	else
		velocidadProyectilX = 0.0004;
}

int Proyectil::getDanio(){
	return danio;
}

bool Proyectil::esVisible(){
	return visible;
}

int Proyectil::getIdTirador(){
	return idTirador;
}

void Proyectil::setVelocidadY(double velocidadProyectilY){
	this->velocidadProyectilY = velocidadProyectilY;
}

void Proyectil::trayectoria(std::vector<Plataforma*>& plataformas) {
	micros t = 0;
	double vx = 0;

	if (tiempoEnMovimiento > 0) {
		vx = velocidadProyectilX;
		t = tiempo() - tiempoEnMovimiento;
		tiempoEnMovimiento += t;
	}

	cambios |= t != 0;

	if (vx != 0)
		orientacion = vx < 0;

	int nuevaPosX = pos.x + (int)round(vx*t);
	int nuevaPosY = pos.y + (int)round(velocidadProyectilY*t);
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
}

Proyectil* Proyectil::crearProyectil(){
	return NULL;
}

bool Proyectil::tieneCambios(std::vector<Plataforma*>& plataformas) {
	std::unique_lock<std::mutex> lock(mutex);
	trayectoria(plataformas);
	return cambios;
}

void Proyectil::setPos(Punto nuevaPosicion) {
	this->pos.x = nuevaPosicion.x;
	this->pos.y = nuevaPosicion.y + 140;
}
