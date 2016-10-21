/*
 * Vista.h
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#ifndef VISTA_H_
#define VISTA_H_

#include "Conexion.h"
#include "Config.h"
#include "Estado.h"
#include "Teclas.h"
#include "Logger.h"
#include "Utils.h"

#include "SDL.h"
#include "SDL_image.h"
#include "NoImage.h"

class Imagen {
public:
	Imagen(Punto tamanioDestino);
	virtual ~Imagen();
	virtual void cargar(SDL_Renderer* renderer)=0;
	virtual int getZindex() = 0;
protected:
	void cargarImagen(SDL_Renderer* renderer, std::array<char, 512>& path);
	Punto tamanio;
	Punto tamanioDestino;
	double escala;
	SDL_Texture* img;
};

class RendererSprite;
class Sprite: public Imagen {
	friend class RendererSprite;
public:
	Sprite(Punto tamanioObj, ConfigSprite& config);
	virtual void cargar(SDL_Renderer* renderer);
	virtual int getZindex();

private:
	ConfigSprite& config;
	double divider;
};

class RendererCapa;
class Capa: public Imagen {
	friend class RendererCapa;
public:
	Capa(Punto ventana, int longitud, ConfigCapa& config);
	virtual void cargar(SDL_Renderer* renderer);
	virtual int getZindex();
private:
	ConfigCapa& config;
	int tiles;
	int longitud;
};

class Renderer {
public:
	virtual ~Renderer();
	virtual int getZindex() = 0;
	virtual void aplicar(SDL_Renderer* renderer) = 0;
};

class RendererSprite : public Renderer {
public:
	RendererSprite(Sprite* sprite, Punto pos, int frame, bool orientacion, bool esJugador);
	virtual int getZindex();
	virtual void aplicar(SDL_Renderer* renderer);

private:
	Sprite* sprite;
	Punto pos;
	int frame;
	bool orientacion;
	bool esJugador;
};

class RendererCapa : public Renderer {
public:
	RendererCapa(Capa* capa, int offsetVista);
	virtual int getZindex();
	virtual void aplicar(SDL_Renderer* renderer);
private:
	Capa* capa;
	int offsetVista;
	SDL_Rect seccion, dest;
};


class Vista {
public:
	Vista(ColaBloqueante<int>& _eventosTeclado, std::string nombreJugador, int idJugador, Config configuracion);
	~Vista();

	void initSDL();

	void iniciar();
	void detener();
	void nuevoEstado(int offsetVista, std::vector<EstadoObj>& estado);

private:
	void cicloPrincipal();
	void enviarEventos();
	void actualizar();

	bool detenido;
	std::string nombreJugador;
	int idJugador;
	Config configuracion;

	Teclas teclas;
	ColaBloqueante<int>& eventosTeclado;

	std::mutex lockEstado;
	int offsetVista;
	std::vector<EstadoObj> estado;

	SDL_Window* ventana;
	SDL_Renderer* renderer;
	std::vector<std::shared_ptr<Capa>> capas;
	HashMap<Estado, std::shared_ptr<Sprite>> sprites;

	std::vector<Renderer*> renderers;
};



#endif /* VISTA_H_ */

