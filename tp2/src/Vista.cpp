/*
 * Vista.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Vista.h"

int framerate = 60;
double frameDelay = 1000000.0/framerate;

Vista::Vista(ColaBloqueante<int>& _eventosTeclado, int idJugador, Config& _configuracion) : eventosTeclado(_eventosTeclado), configuracion(_configuracion) {
	ventana = nullptr;
	renderer = nullptr;
	surface = nullptr;
	detenido = false;
	this->idJugador = idJugador;
}

Vista::~Vista() {
	SDL_DestroyWindow(ventana);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Vista::iniciar() {
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	} else {
		//Create window
		ventana = SDL_CreateWindow("TP2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				configuracion.getTamanioVentana().x, configuracion.getTamanioVentana().y,
				SDL_WINDOW_SHOWN);
		if (ventana == nullptr) {
			error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
		} else {
			renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
			cicloPrincipal();
		}
	}
}

void Vista::detener() {
	detenido = true;
}

void Vista::cicloPrincipal() {
	while (! detenido) {
		micros t = tiempo();
		enviarEventos();
		actualizar();
		t = (micros) frameDelay - (tiempo() - t);
		if (t > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds((long) t/1000));
	}
}

void Vista::nuevoEstado(double desplazamiento, std::vector<EstadoObj>& estado) {
	lockEstado.lock();
	this->desplazamiento = desplazamiento;
	this->estado.clear();
	this->estado = estado;
	lockEstado.unlock();
}

void Vista::enviarEventos() {
	SDL_Event evento;
	while (SDL_PollEvent(&evento)) {
		if (evento.type == SDL_QUIT) {
			detener();
			break;
		} else if (evento.type == SDL_KEYUP || evento.type == SDL_KEYDOWN) {
			bool cambio = false;
			switch (evento.key.keysym.sym) {
			case SDLK_UP:
				cambio = teclas.evento(ARRIBA, evento.type == SDL_KEYDOWN);
				break;
			case SDLK_DOWN:
				break;
			case SDLK_LEFT:
				cambio = teclas.evento(IZQ, evento.type == SDL_KEYDOWN);
				break;
			case SDLK_RIGHT:
				cambio = teclas.evento(DER, evento.type == SDL_KEYDOWN);
				break;
			case SDLK_r:
				cambio = teclas.evento(R, evento.type == SDL_KEYDOWN);
				break;
			default:
				break;
			}

			if (cambio)
				eventosTeclado.encolar(teclas.getEstado());
		}
	}
}

void Vista::actualizar() {
	lockEstado.lock();

	if (estado.size() > 0) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		info("Desplazamiento fondo " + std::to_string((int) (desplazamiento * 100)) +"%", true);

		auto it = estado.begin();
		while (it != estado.end()) {
			//info(it->toString(), true);

			SDL_Rect rect;
			rect.w = it->getTamanio().x;
			rect.h = it->getTamanio().y;
			rect.x = it->getPos().x;
			rect.y = it->getPos().y;

			if (it->getId() == idJugador)
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rect);

			estado.erase(it);
		}
		SDL_RenderPresent(renderer);
	}

	lockEstado.unlock();
}
