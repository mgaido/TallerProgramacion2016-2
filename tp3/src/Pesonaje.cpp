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
		Punto pos;
		pos.x = getPos().x + (getOrientacion() ? 0 : getTamanio().x);
		pos.y = getPos().y - getTamanio().y * 0.7;
		nuevoProyectil->setPos(pos);
		nuevoProyectil->setOrientacion(orientacion);
		if ((velocSaltoY < 0) && nuevoProyectil->getTipo() == Tipo::GunH)
			nuevoProyectil->setVelocidadY(-0.00008);
		else if((velocSaltoY > 0) && nuevoProyectil->getTipo() == Tipo::GunH)
			nuevoProyectil->setVelocidadY(0.00008);
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
	actualizar(std::vector<Plataforma*>());
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

bool Personaje::tieneCambios(std::vector<Plataforma*>& plataformas) {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar(plataformas);
}

bool Personaje::actualizar(std::vector<Plataforma*>& plataformas) {
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


		pos.x  = pos.x + (int)round(vx*t);
		pos.y = pos.y + (int)round(velocSaltoY*t);


		Plataforma* plataforma = nullptr;
		auto it = plataformas.begin();
		while (it != plataformas.end()) {
			Plataforma* unaPlataforma = *it;
			int inicio = unaPlataforma->getPos().x;
			int fin = inicio + unaPlataforma->getTamanio().x;
			int posX = pos.x + getTamanio().x / 2;
			if ( posX >= inicio && posX <= fin ) {
				plataforma = unaPlataforma;
				break;
			}
			it++;
		}

		bool cayendo = tiempoSalto > 0 && velocSaltoY < 0;
		if	(cayendo) {
			if (pos.y <= 0) {
				pos.y = 0;
				cayendo = false;
			}

			if (plataforma != nullptr && getPos().y <= plataforma->getPos().y && getPos().y > plataforma->getPos().y - plataforma->getTamanio().y - 10) {
				pos.y = plataforma->getPos().y;
				cayendo = false;
			}

			if (! cayendo) {
				velocSaltoX = 0;
				velocSaltoY = 0;
				if (velocCaminar != 0)
					tiempoCaminando = tiempoSalto;
				tiempoSalto = 0;
			}

		} else if (tiempoCaminando > 0 && tiempoSalto == 0 && pos.y > 0 && plataforma == nullptr) {
			velocSaltoX = velocCaminar;
			velocSaltoY = 0;
			tiempoSalto = tiempo();
			tiempoCaminando = 0;
			cayendo = true;
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
