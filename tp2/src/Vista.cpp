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

		auto capaIt = configuracion.getConfigCapas().begin();
		while (capaIt != configuracion.getConfigCapas().end()) {
			std::shared_ptr<Capa> capa = std::make_shared<Capa>(configuracion.getTamanioVentana(),
					configuracion.getLongitud(), *capaIt);
			capa->cargar(renderer);
			capas.push_back(capa);
			capaIt++;
		}

		auto spriteIt = configuracion.getConfigSprites().begin();
		while (spriteIt != configuracion.getConfigSprites().end()) {
			std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(configuracion.getTamanioJugador(), *spriteIt);
			sprite->cargar(renderer);
			sprites[spriteIt->estado] = sprite;
			spriteIt++;
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

		std::vector<Renderer*> renderers;

		auto capa = capas.begin();
		while (capa != capas.end()) {
			renderers.push_back(new RendererCapa(capa->get(), offsetVista));
			capa++;
		}

		auto it = estado.begin();
		while (it != estado.end()) {
			auto configSprite = sprites[it->getEstado()];
			renderers.push_back(new RendererSprite(configSprite.get(), it->getPos(),
					it->getFrame(), it->getOrientacion(), it->getId() == idJugador));
			estado.erase(it);
		}

		sort(renderers.begin(), renderers.end(), [](Renderer* a, Renderer* b) -> bool {
		    return a->getZindex() < b->getZindex();
		});

		auto rendererIt = renderers.begin();
		while (rendererIt != renderers.end()) {
			(*rendererIt)->aplicar(renderer);
			delete *rendererIt;
			renderers.erase(rendererIt);
		}

		SDL_RenderPresent(renderer);
	}

	lockEstado.unlock();
}

Imagen::Imagen(Punto tamanioDestino){
	img = nullptr;
	this->tamanioDestino = tamanioDestino;
}

Imagen::~Imagen() {
	if (img)
		SDL_DestroyTexture(img);
}

void Imagen::cargarImagen(SDL_Renderer* renderer, std::array<char, 512>& _path) {
	std::string path = std::string(_path.data());
	SDL_Surface* imagen = IMG_Load(path.c_str());
	if (! imagen) {
		error( "No se pudo cargar la imagen \"" + path + "\". Error: " + std::string(IMG_GetError()), true);

		SDL_RWops *rw = SDL_RWFromMem((void*) &IMAGE_DATA, sizeof IMAGE_DATA);
		imagen = IMG_Load_RW(rw, 1);
	}
	tamanio.x = imagen->w;
	tamanio.y = imagen->h;

	escala = (double) tamanio.y / tamanioDestino.y;

	img = SDL_CreateTextureFromSurface(renderer, imagen);
	SDL_FreeSurface(imagen);
}

Capa::Capa(Punto ventana, int longitud, ConfigCapa& _config) : Imagen(ventana), config(_config) {
	this->longitud = longitud;
}

void Capa::cargar(SDL_Renderer* renderer) {
	cargarImagen(renderer, config.imagen);
}

int Capa::getZindex() {
	return config.zIndex;
}

Sprite::Sprite(Punto tamanioObj, ConfigSprite& _config) : Imagen(tamanioObj), config(_config) {
}

void Sprite::cargar(SDL_Renderer* renderer) {
	cargarImagen(renderer, config.imagen);
}

int Sprite::getZindex() {
	return config.zIndex;
}

RendererCapa::RendererCapa(Capa* capa, int offsetVista) {
	this->capa = capa;
	this->offsetVista = offsetVista;

	seccion.y = 0;
	seccion.h = capa->tamanio.y;
	dest.y = 0;
	dest.h = capa->tamanioDestino.y;
}

int RendererCapa::getZindex() {
	return capa->getZindex();
}

void RendererCapa::aplicar(SDL_Renderer* renderer) {
	seccion.x = (int) capa->tamanio.x * (double) offsetVista / capa->longitud;
	seccion.w = std::min<int>(capa->tamanioDestino.x * capa->escala, capa->tamanio.x - seccion.x);
	dest.x = 0;
	dest.w = seccion.w / capa->escala;

	SDL_RenderCopy(renderer, capa->img, &seccion, &dest);

	if (dest.w < capa->tamanioDestino.x) {
		seccion.x = 0;
		seccion.w = capa->tamanioDestino.x * capa->escala - seccion.w;
		dest.x = dest.w;
		dest.w = seccion.w / capa->escala;

		SDL_RenderCopy(renderer, capa->img, &seccion, &dest);
	}
}

RendererSprite::RendererSprite(Sprite* sprite, Punto pos, int frame, bool orientacion, bool esJugador) {
	this->sprite = sprite;
	this->pos = pos;
	int divider = framerate / sprite->config.frames;
	this->frame = frame/divider % sprite->config.frames;
	this->orientacion = orientacion;
	this->esJugador = esJugador;
}

int RendererSprite::getZindex() {
	return sprite->getZindex() + (esJugador ? 1 : 0);
}

void RendererSprite::aplicar(SDL_Renderer* renderer) {

	SDL_Rect seccion;
	seccion.x = (sprite->tamanio.x / sprite->config.frames) * frame;
	seccion.y = 0;
	seccion.w = sprite->tamanio.x / sprite->config.frames;
	seccion.h = sprite->tamanio.y;

	SDL_Rect rect;
	rect.w = sprite->tamanioDestino.x;
	rect.h = sprite->tamanioDestino.y;
	rect.x = pos.x;
	rect.y = pos.y;

	if (orientacion)
	    SDL_RenderCopyEx(renderer, sprite->img, &seccion, &rect, 0, 0, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(renderer, sprite->img, &seccion, &rect);
}

