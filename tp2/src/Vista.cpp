/*
 * Vista.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#include "Vista.h"

double frameDelay;

Vista::Vista(ColaBloqueante<int>& _eventosTeclado, std::string nombreJugador, int idJugador, Config configuracion)
															: eventosTeclado(_eventosTeclado) {
	ventana = nullptr;
	renderer = nullptr;
	detenido = false;
	this->idJugador = idJugador;
	this->nombreJugador = nombreJugador;
	this->configuracion = configuracion;
	frameDelay = 1000000.0/configuracion.getFrameRate();
	initSDL();
}

Vista::~Vista() {
	auto rendererIt = renderers.begin();
	while (rendererIt != renderers.end()) {
		delete *rendererIt;
		renderers.erase(rendererIt);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ventana);
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
	renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
			/*case SDLK_q:
				detener();
				break;*/
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

		auto rendererIt = renderers.begin();
		while (rendererIt != renderers.end()) {
			(*rendererIt)->aplicar(renderer);
			delete *rendererIt;
			renderers.erase(rendererIt);
		}

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
	}

	auto rendererIt = renderers.begin();
	while (rendererIt != renderers.end()) {
		(*rendererIt)->aplicar(renderer);
		rendererIt++;
	}

	SDL_RenderPresent(renderer);

	lockEstado.unlock();
}

Imagen::Imagen(Punto tamanioDestino){
	img = nullptr;
	this->tamanioDestino = tamanioDestino;
	this->escala = 0;
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

	SDL_SetColorKey(imagen, SDL_TRUE, SDL_MapRGB(imagen->format, 0, 255, 255));

	escala = (double) tamanio.y / tamanioDestino.y;

	img = SDL_CreateTextureFromSurface(renderer, imagen);
	SDL_FreeSurface(imagen);
}

Capa::Capa(Punto ventana, int longitud, ConfigCapa& _config) : Imagen(ventana), config(_config) {
	this->longitud = longitud;
	this->tiles = 0;
}

void Capa::cargar(SDL_Renderer* renderer) {
	cargarImagen(renderer, config.imagen);
	this->tiles = 1;
	if (tamanioDestino.x > tamanio.x)
		this->tiles = ceil(tamanioDestino.x / (double) tamanio.x);
}

int Capa::getZindex() {
	return config.zIndex;
}

Sprite::Sprite(Punto tamanioObj, ConfigSprite& _config) : Imagen(tamanioObj), config(_config) {
	divider = std::max<double>(1, 1000 * config.tiempo / (frameDelay) / config.frames);
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
	dest.x = 0;
	while (dest.x < capa->tamanioDestino.x) {
		if (dest.x == 0) {
			//seccion.x = (int) ((capa->tamanio.x * capa->tiles - capa->tamanioDestino.x * capa->escala) * (double) offsetVista / capa->longitud) % capa->tamanio.x;
			seccion.x = (int) (capa->tamanio.x * capa->tiles * (double) offsetVista / capa->longitud) % capa->tamanio.x;
		} else
			seccion.x = 0;

		double ancho = std::min<double>(capa->tamanioDestino.x * capa->escala, capa->tamanio.x - seccion.x);

		seccion.w = round(ancho);
		dest.w = round(ancho / capa->escala);

		SDL_RenderCopy(renderer, capa->img, &seccion, &dest);
		dest.x += dest.w;
	}
}

RendererSprite::RendererSprite(Sprite* sprite, Punto pos, int frame, bool orientacion, bool esJugador) {
	this->sprite = sprite;
	this->pos = pos;
	this->frame = frame;

	this->orientacion = orientacion;
	this->esJugador = esJugador;
}

int RendererSprite::getZindex() {
	return sprite->getZindex() + (esJugador ? 1 : 0);
}

void RendererSprite::aplicar(SDL_Renderer* renderer) {

	int frame = (int) (this->frame++ / sprite->divider) % sprite->config.frames;

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

	if (this->esJugador) {
		SDL_Rect indicador;
		indicador.w = rect.w / 2;
		indicador.h = rect.h / 10;
		indicador.x = pos.x + indicador.w / 2;
		indicador.y = pos.y - indicador.h - rect.h * 0.05;

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 50);
		SDL_RenderFillRect(renderer, &indicador);
	}

	if (orientacion)
	    SDL_RenderCopyEx(renderer, sprite->img, &seccion, &rect, 0, 0, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(renderer, sprite->img, &seccion, &rect);
}

Renderer::~Renderer() {}

