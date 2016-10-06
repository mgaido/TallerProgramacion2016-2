/*
 * Vista.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Vista.h"

int delay = 1000/60.0;

Vista::Vista(ColaBloqueante<int>& _eventosTeclado) : eventosTeclado(_eventosTeclado) {
	ventana = nullptr;
	renderer = nullptr;
	surface = nullptr;
	detenido = false;
}

Vista::~Vista() {
}

void Vista::iniciar() {
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	} else {
		//Create window
		ventana = SDL_CreateWindow("TP2", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);
		if (ventana == nullptr) {
			error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
		} else {
			renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
			clear();
			cicloPrincipal();
		}
	}
}

void Vista::clear() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for(auto kv : objs) {
	    SDL_RenderFillRect(renderer, &kv.second);
	}
	SDL_RenderPresent(renderer);
}

void Vista::detener() {
	if (! detenido) {
		detenido = true;
		SDL_DestroyWindow(ventana);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}
}

void Vista::cicloPrincipal() {
	while (! detenido) {
		millis t = tiempo();
		enviarEventos();
		actualizar();
		t = delay - (tiempo() - t);
		if (t > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(t));
	}
}

void Vista::recibirActualizaciones(std::vector<Actualizacion>& actualizaciones) {
	lockActualizaciones.lock();
	this->actualizaciones.reserve(this->actualizaciones.size() + actualizaciones.size());
	std::move(actualizaciones.begin(), actualizaciones.end(), std::inserter(this->actualizaciones, this->actualizaciones.end()));
	actualizaciones.clear();
	lockActualizaciones.unlock();
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
			default:
				break;
			}

			if (cambio)
				eventosTeclado.encolar(teclas.getEstado());
		}
	}
}

void Vista::actualizar() {
	lockActualizaciones.lock();

	auto it = actualizaciones.begin();
	while (it != actualizaciones.end()) {
		info(it->toString());

		if (it->getEvento() == Evento::Agregar) {
			SDL_Rect rect;
			rect.w = it->getTamanio().x;
			rect.h = it->getTamanio().y;
			rect.x = it->getPos().x;
			rect.y = 400 - it->getPos().y;
			objs[it->getId()] = rect;
		} else if (it->getEvento() == Evento::Modificar) {
			SDL_Rect rect = objs[it->getId()];
			rect.x = it->getPos().x;
			rect.y = 400 - it->getPos().y;
			objs[it->getId()] = rect;
		}

		actualizaciones.erase(it);
	}
	lockActualizaciones.unlock();

	clear();
}
