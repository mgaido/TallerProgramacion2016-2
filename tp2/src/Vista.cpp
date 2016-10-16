/*
 * Vista.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Vista.h"

int framerate = 60;
double frameDelay = 1000000.0/framerate;

Vista::Vista(ColaBloqueante<int>& _eventosTeclado, std::string nombreJugador, int idJugador, Config configuracion)
															: eventosTeclado(_eventosTeclado) {
	ventana = nullptr;
	renderer = nullptr;
	detenido = false;
	this->idJugador = idJugador;
	this->nombreJugador = nombreJugador;
	this->configuracion = configuracion;

	initSDL();
}

Vista::~Vista() {
	SDL_DestroyWindow(ventana);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}

void Vista::initSDL() {
	if (SDL_Init( SDL_INIT_VIDEO) < 0)
		error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
		error( "SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
}

void Vista::iniciar() {
	//Create window
	ventana = SDL_CreateWindow(nombreJugador.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			configuracion.getTamanioVentana().x, configuracion.getTamanioVentana().y,
			SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

	if (ventana == nullptr) {
		error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
	} else {

		auto it = configuracion.getConfigCapas().begin();
		while (it != configuracion.getConfigCapas().end()) {
			std::shared_ptr<Capa> capa = std::make_shared<Capa>();
			capa->cargar(configuracion.getTamanioVentana(), configuracion.getLongitud(),  *it, renderer);
			capas.push_back(capa);
			it++;
		}

		cicloPrincipal();
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

void Vista::nuevoEstado(int offsetVista, std::vector<EstadoObj>& estado) {
	lockEstado.lock();
	this->offsetVista = offsetVista;
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

		auto capa = capas.begin();
		while (capa != capas.end()) {
			(*capa)->aplicar(renderer, offsetVista);
			capa++;
		}

		auto it = estado.begin();
		while (it != estado.end()) {
			//info(it->toString(), true);

			SDL_Rect rect;
			rect.w = it->getTamanio().x;
			rect.h = it->getTamanio().y;
			rect.x = it->getPos().x;
			rect.y = it->getPos().y;

			if (it->getEstado() == Estado::Caminando)
				SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
			if (it->getEstado() == Estado::Saltando)
				SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
			if (it->getEstado() == Estado::Quieto)
				SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
			if (it->getEstado() == Estado::Desconectado)
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

			SDL_RenderFillRect(renderer, &rect);

			estado.erase(it);
		}
		SDL_RenderPresent(renderer);
	}

	lockEstado.unlock();
}

Capa::Capa(){
	img = nullptr;
}

Capa::~Capa(){
	SDL_DestroyTexture(img);
}

void Capa::cargar(Punto ventana, int longitud, ConfigCapa& config, SDL_Renderer* renderer) {
	this->ventana = ventana;
	this->longitud = longitud;
	std::string path = std::string(config.imagen.data());

	SDL_Surface* imagen = IMG_Load(path.c_str());
	img = SDL_CreateTextureFromSurface(renderer, imagen);

	if (imagen) {
		tamanio.x = imagen->w;
		tamanio.y = imagen->h;
		escala = (double) tamanio.y / ventana.y;
		zIndex = config.zIndex;
		seccion.y = 0;
		seccion.h = tamanio.y;
		dest.y = 0;
		dest.h = ventana.y;

		img = SDL_CreateTextureFromSurface(renderer, imagen);
		SDL_FreeSurface(imagen);
	} else {
		error( "No se pudo cargar la imagen " + path + ". Error: " + std::string(IMG_GetError()), true);
	}
}

void Capa::aplicar(SDL_Renderer* renderer, int offsetVista) {

	seccion.x = (int) tamanio.x * (double) offsetVista / longitud;
	seccion.w = std::min<int>(ventana.x * escala, tamanio.x - seccion.x);
	dest.x = 0;
	dest.w = seccion.w / escala;

	SDL_RenderCopy(renderer, img, &seccion, &dest);

	if (dest.w < ventana.x) {
		seccion.x = 0;
		seccion.w = ventana.x * escala - seccion.w;
		dest.x = dest.w;
		dest.w = seccion.w / escala;

		SDL_RenderCopy(renderer, img, &seccion, &dest);
	}
}
