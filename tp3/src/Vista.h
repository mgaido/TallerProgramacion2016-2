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
#include "SDL_ttf.h"
#include "NoImage.h"

class Imagen {
public:
	Imagen();
	virtual ~Imagen();
	virtual void cargar(SDL_Renderer* renderer)=0;
	virtual int getZindex() = 0;
protected:
	void cargarImagen(SDL_Renderer* renderer, std::string imagen);
	Punto tamanio;
	SDL_Texture* img;
};


class RendererPantalla;
class Pantalla: public Imagen {
	friend class RendererPantalla;
public:
	Pantalla(std::string imagen, Punto ventana);
	virtual void cargar(SDL_Renderer* renderer);
	virtual int getZindex();
private:
	std::string imagen;
	Punto ventana;
};

class RendererSprite;
class Sprite: public Imagen {
	friend class RendererSprite;
public:
	Sprite(ConfigSprite& config);
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
	Punto tamanioDestino;
	double escala;
	int tiles;
	int longitud;
};

class Renderer {
public:
	virtual ~Renderer();
	virtual int getZindex() = 0;
	virtual void aplicar(SDL_Renderer* renderer, TTF_Font* fuente) = 0;
};

class RendererPantalla : public Renderer {
public:
	RendererPantalla(Pantalla* pantalla);
	virtual int getZindex();
	virtual void aplicar(SDL_Renderer* renderer, TTF_Font* fuente);
private:
	Pantalla* pantalla;
};

class RendererSprite : public Renderer {
public:
	RendererSprite(Sprite* sprite, Punto pos, Punto tamanio, std::string texto, int frame, bool orientacion, double rotacion, bool esJugador);
	virtual int getZindex();
	virtual void aplicar(SDL_Renderer* renderer, TTF_Font* fuente);

private:
	void dibujarTexto(SDL_Renderer* renderer, TTF_Font* fuente);


	Sprite* sprite;
	Punto pos;
	Punto tamanio;
	std::string texto;
	int frame;
	bool orientacion;
	double rotacion;
	bool esJugador;
};

class RendererCapa : public Renderer {
public:
	RendererCapa(Capa* capa, int offsetVista);
	virtual int getZindex();
	virtual void aplicar(SDL_Renderer* renderer, TTF_Font* fuente);
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
	void nuevoEstado(EstadoJuego estado, int offsetVista, std::vector<EstadoObj>& estadoObjs, std::vector<InfoJugador>& hudInfo);

private:
	void cicloPrincipal();
	void enviarEventos();
	void actualizar();

	void mostrarHud();
	void escribirLineaHud(int linea, int jugador, int jugadores, std::string texto, SDL_Color color);

	bool detenido;
	std::string nombreJugador;
	int idJugador;
	Config configuracion;

	Teclas teclas;
	ColaBloqueante<int>& eventosTeclado;

	std::mutex lockEstado;
	EstadoJuego estado;
	int offsetVista;
	std::vector<EstadoObj> estadoObjs;
	std::vector<InfoJugador> hudInfo;

	SDL_Window* ventana;
	SDL_Renderer* renderer;
	std::vector<std::shared_ptr<Capa>> capas;
	HashMap<Tipo, HashMap<Estado, std::shared_ptr<Sprite>>> spritess;
	Pantalla* espera;
	Pantalla* gameOver;

	std::vector<Renderer*> renderers;

	TTF_Font* fuente;
};



#endif /* VISTA_H_ */

