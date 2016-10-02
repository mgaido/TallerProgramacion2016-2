/*
 * Vista.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Vista.h"

Vista::Vista(ColaBloqueante<int>& _eventosTeclado) : eventosTeclado(_eventosTeclado) {
	ventana = nullptr;
	detenido = false;
}

Vista::~Vista() {
}

void Vista::iniciar() {
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	} else {
		//Create window
		ventana = SDL_CreateWindow("TP2", SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);
		if (ventana == nullptr) {
			error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
		} else {
			SDL_Surface* screenSurface = SDL_GetWindowSurface(ventana);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(ventana);

			leerEventos();
		}
	}
}

void Vista::detener() {
	if (! detenido) {
		detenido = true;
		SDL_DestroyWindow(ventana);
		SDL_Quit();
	}
}

void Vista::leerEventos() {
	SDL_Event evento;
	Teclas teclas;
	while (! detenido) {
		while (SDL_WaitEvent(&evento)) {
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
}

