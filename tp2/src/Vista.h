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

class Capa {
public:
	Capa();
	~Capa();
	void cargar(Punto ventana, int longitud, ConfigCapa& config, SDL_Renderer* renderer);
	void aplicar(SDL_Renderer* surface, int offsetVista);
private:
	Punto tamanio;
	int zIndex;
	Punto ventana;
	int longitud;
	double escala;
	SDL_Texture* img;
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
};



#endif /* VISTA_H_ */

