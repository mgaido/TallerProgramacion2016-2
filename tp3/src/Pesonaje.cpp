#include "Personaje.h"
#include "GunH.h"


Personaje::Personaje(int id, Config & _configuracion) : Objeto(id), configuracion(_configuracion) {
	energia = 1000;
	arma =  new GunH(++contador,id);  //agregar id  //La por default es GunH Cambiar
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

Proyectil* Personaje::disparar() {
	Proyectil* nuevoProyectil = arma->disparar();
	if (nuevoProyectil != NULL) {
		nuevoProyectil->setPos(this->getPos());
		nuevoProyectil->setOrientacion(orientacion);
	}
	return nuevoProyectil;
}

Proyectil* Personaje::dispararDirigido(Objeto* enemigoMasCercano) {
	Proyectil* nuevoProyectil = arma->dispararEspecial(enemigoMasCercano);
	if (nuevoProyectil != NULL) {
		nuevoProyectil->setPos(this->getPos());
		nuevoProyectil->setOrientacion(orientacion);
	}
	return nuevoProyectil;
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
	actualizar(NULL);
	velocCaminar = 0;
	tiempoCaminando = 0;
}

double Personaje::getVelocidadCaminar() {
	return velocCaminar;
}

void Personaje::saltar() {
	std::unique_lock<std::mutex> lock(mutex);

	if (tiempoSalto == 0) {
		velocSaltoY = configuracion.getVelocidadY();
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
}

bool Personaje::tieneCambios(std::vector<Plataforma*>* plataformas) {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar(plataformas);
}

bool Personaje::actualizar(std::vector<Plataforma*>* plataformas) {
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
		if (plataformas != NULL) {
			int nuevaPosX = pos.x + (int)round(vx*t);
			int nuevaPosY = pos.y + (int)round(velocSaltoY*t);

			bool colisionan = false;
			auto it = (*plataformas).begin();
			while ((it != (*plataformas).end()) && !colisionan) {
				Plataforma* unaPlataforma = *it;
				if (((nuevaPosX + getTamanio().x) < (unaPlataforma->getPos().x)) || (nuevaPosX > unaPlataforma->getPos().x + unaPlataforma->getTamanio().x)) {
					colisionan = false;
				}
				else if (((nuevaPosY + getTamanio().y) < (unaPlataforma->getPos().y)) || (nuevaPosY > unaPlataforma->getPos().y + unaPlataforma->getTamanio().y)) {
					colisionan = false;
				}
				else { colisionan = true; }
			}

			if (!colisionan) {
				pos.x = nuevaPosX;
				pos.y = nuevaPosY;
			}
		}else {		
			pos.x += (int)round(vx*t);
			pos.y += (int)round(velocSaltoY*t);
		}
		if	(tiempoSalto > 0 && velocSaltoY < 0 && pos.y <= 0) {
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
