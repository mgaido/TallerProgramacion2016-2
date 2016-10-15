/*
 * Vista.h
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#ifndef VISTA_H_
#define VISTA_H_

#include "Conexion.h"
#include "Estado.h"
#include "Teclas.h"
#include "Logger.h"
#include "Utils.h"

#include "SDL.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Vista {
public:
	Vista(ColaBloqueante<int>& _eventosTeclado);
	~Vista();

	void iniciar();
	void detener();
	void recibirEstado(std::vector<EstadoObj>& estado);

private:
	void cicloPrincipal();
	void enviarEventos();
	void actualizar();

	bool detenido;

	Teclas teclas;
	ColaBloqueante<int>& eventosTeclado;

	std::mutex lockEstado;
	std::vector<EstadoObj> estado;

	SDL_Window* ventana;
	SDL_Renderer* renderer;
	SDL_Surface* surface;

	std::unordered_map<int, SDL_Rect> objs;
};



#endif /* VISTA_H_ */

