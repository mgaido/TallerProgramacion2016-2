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
	TTF_CloseFont(fuente);
	IMG_Quit();
	SDL_Quit();
}

void Vista::initSDL() {
	if (SDL_Init( SDL_INIT_VIDEO) < 0)
		error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
		error( "SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));

	if (TTF_Init() < 0)
		error( "SDL_ttf could not initialize! SDL_ttf Error: " + std::string(TTF_GetError()));
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

		auto spriteIt = configuracion.getConfigSprites().begin();			//modificar xml para gregar config de sprites de enemigos y proyectiles 
		while (spriteIt != configuracion.getConfigSprites().end()) {
			std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(*spriteIt);
			sprite->cargar(renderer);
			spritess[spriteIt->tipo][spriteIt->estado] = sprite;
			spriteIt++;
		}

		fuente = TTF_OpenFont("img/arcade_r.ttf", 20);
		if (! fuente) {
			error( "No se pudo cargar la fuente. Error: " + std::string(TTF_GetError()), true);
			detener();
			return;
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
				cambio = teclas.evento(ABAJO, evento.type == SDL_KEYDOWN);
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
			case SDLK_SPACE:
				cambio = teclas.evento(SPACE, evento.type == SDL_KEYDOWN);
				break;
			case SDLK_z: {
				cambio = teclas.evento(Z, evento.type == SDL_KEYDOWN);
				break;
			}
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
			(*rendererIt)->aplicar(renderer, fuente);
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

			bool encontrado = false;
			auto map = spritess.find(it->getTipo());
			if (map != spritess.end()) {
				auto map2 = map->second.find(it->getEstado());
				if (map2 != map->second.end()) {
					encontrado = true;
					auto configSprite = map2->second;
					renderers.push_back(new RendererSprite(configSprite.get(), it->getPos(), it->getTamanio(), it->getNombre(), it->getFrame(), it->getOrientacion(), it->getId() == idJugador));
				}
			}
			if (! encontrado) {
				std::cerr << "Falta sprite " << it->toString() << std::endl;
			}

			estado.erase(it);
		}

		sort(renderers.begin(), renderers.end(), [](Renderer* a, Renderer* b) -> bool {
			return a->getZindex() < b->getZindex();
		});
	}

	auto rendererIt = renderers.begin();
	while (rendererIt != renderers.end()) {
		(*rendererIt)->aplicar(renderer, fuente);
		rendererIt++;
	}

	SDL_RenderPresent(renderer);

	lockEstado.unlock();
}

Imagen::Imagen(){
	img = nullptr;
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

	img = SDL_CreateTextureFromSurface(renderer, imagen);
	SDL_FreeSurface(imagen);
}

Capa::Capa(Punto ventana, int longitud, ConfigCapa& _config) : Imagen(), config(_config) {
	this->tamanioDestino = ventana;
	this->longitud = longitud;
	this->tiles = 1;
}

void Capa::cargar(SDL_Renderer* renderer) {
	cargarImagen(renderer, config.imagen);
	this->escala = (double) tamanio.y / tamanioDestino.y;
	if (tamanioDestino.x * escala > tamanio.x)
		this->tiles = ceil(tamanioDestino.x / (double) tamanio.x);

}

int Capa::getZindex() {
	return config.zIndex;
}

Sprite::Sprite(ConfigSprite& _config) : Imagen(), config(_config) {
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

void RendererCapa::aplicar(SDL_Renderer* renderer, TTF_Font* fuente) {
	dest.x = 0;
	while (dest.x < capa->tamanioDestino.x) {
		if (dest.x == 0) {
			seccion.x = (int) ((capa->tamanio.x * capa->tiles - capa->tamanioDestino.x * capa->escala) * (double) offsetVista / capa->longitud) % capa->tamanio.x;
			//seccion.x = (int) (capa->tamanio.x * capa->tiles * (double) offsetVista / capa->longitud) % capa->tamanio.x;
		} else
			seccion.x = 0;

		double ancho = std::min<double>(capa->tamanioDestino.x * capa->escala, capa->tamanio.x - seccion.x);

		seccion.w = round(ancho);
		dest.w = round(ancho / capa->escala);

		SDL_RenderCopy(renderer, capa->img, &seccion, &dest);
		dest.x += dest.w;
	}
}

RendererSprite::RendererSprite(Sprite* sprite, Punto pos, Punto tamanio, std::string texto, int frame, bool orientacion, bool esJugador) {
	this->sprite = sprite;
	this->pos = pos;
	this->tamanio = tamanio;
	this->texto = texto;
	this->frame = frame;
	this->orientacion = orientacion;
	this->esJugador = esJugador;
}

int RendererSprite::getZindex() {
	return sprite->getZindex() + (esJugador ? 1 : 0);
}

void RendererSprite::aplicar(SDL_Renderer* renderer, TTF_Font* fuente) {

	int frame = (int) (this->frame++ / sprite->divider) % sprite->config.frames;

	SDL_Rect seccion;
	seccion.x = (sprite->tamanio.x / sprite->config.frames) * frame;
	seccion.y = 0;
	seccion.w = sprite->tamanio.x / sprite->config.frames;
	seccion.h = sprite->tamanio.y;

	SDL_Rect rect;

	double anchoEscalado = tamanio.y / (double) sprite->tamanio.y * sprite->tamanio.x;
	if (anchoEscalado < tamanio.x)
		rect.w = anchoEscalado;
	else
		rect.w = tamanio.x;

	rect.h = tamanio.y;
	rect.y = pos.y;
	rect.x = pos.x;

	do {
		if (orientacion)
		    SDL_RenderCopyEx(renderer, sprite->img, &seccion, &rect, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopy(renderer, sprite->img, &seccion, &rect);

		rect.x += rect.w;
		rect.w = std::min<int>(anchoEscalado, tamanio.x - (rect.x - pos.x));

	} while (rect.x < pos.x + tamanio.x);

	if (texto.length() > 0)
		dibujarTexto(renderer, fuente);
}


void RendererSprite::dibujarTexto(SDL_Renderer* renderer, TTF_Font* fuente) {

	SDL_Color color;
	if (sprite->config.tipo == Tipo::Jugador) {
		if (esJugador)
			color = { 255, 255, 180, 255 };
		else
			color = { 180, 180, 180, 255 };
	} else
		color = { 255, 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderText_Solid(fuente, texto.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	int ancho, alto;
	SDL_QueryTexture( texture, NULL, NULL, &ancho, &alto);

	SDL_Rect label;
	label.w = std::min<int>(ancho, tamanio.x);
	label.h = alto / (double) ancho * label.w;

	label.x = pos.x + (tamanio.x - label.w) / 2;
	label.y = pos.y - label.h - pos.y * 0.01;

	SDL_RenderCopy(renderer, texture, nullptr, &label);
	SDL_DestroyTexture(texture);
}


Renderer::~Renderer() {
}

