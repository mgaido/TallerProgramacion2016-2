#include "Soldado.h"
#include "Juego.h"

Soldado::Soldado(int id, Config& _configuracion) : Personaje(id, _configuracion) {
	tamanio.x = configuracion.getTamanioJugador().x;
	tamanio.y = configuracion.getTamanioJugador().y;

	velocCaminar = 0, velocSaltoX = 0, velocSaltoY = 0;
	tiempoCaminando = 0;
	tiempoSalto = 0;
	apunta = NEUTRO;
}

void Soldado::caminar(Direccion direccion) {
	std::unique_lock<std::mutex> lock(mutex);

	if (direccion == Direccion::IZQUIERDA)
		velocCaminar = -configuracion.getVelocidadX();
	else
		velocCaminar = configuracion.getVelocidadX();

	if (tiempoSalto == 0)
		tiempoCaminando = tiempo();
}

void Soldado::detenerse() {
	std::unique_lock<std::mutex> lock(mutex);
	velocCaminar = 0;
	tiempoCaminando = 0;
}

double Soldado::getVelocidadCaminar() {
	return velocCaminar;
}

void Soldado::apuntar(char direcion){
	apunta = direcion;
	if (direcion == UP) {
		//estado = Estado::MirarAbajo;
	} else if(direcion == DOWN) {
		//estado = Estado::MirarAbajo;
	}
}

void Soldado::saltar() {
	std::unique_lock<std::mutex> lock(mutex);
	if (tiempoSalto == 0) {
		velocSaltoY = configuracion.getVelocidadY();
		velocSaltoX = velocCaminar;
		tiempoSalto = tiempo();
	}
}

bool Soldado::tieneCambios(Juego* juego) {
	std::unique_lock<std::mutex> lock(mutex);
	return actualizar(juego);
}

bool Soldado::actualizar(Juego* juego) {
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
		auto it = juego->getPlataformas().begin();
		while (it != juego->getPlataformas().end()) {
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
			estado = Estado::Normal;

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

Proyectil* Soldado::disparar(Juego* juego) {
	Proyectil* proyectil = Personaje::disparar(juego);
	if (proyectil != nullptr) {
		if ((apunta == DISPARAR_ABAJO && pos.y > 0) || apunta == DISPARAR_ARRIBA)
			proyectil->setOrientacionY(apunta);
	}

	return proyectil;
}
