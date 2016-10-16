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

class Vista {
public:
	Vista(ColaBloqueante<int>& _eventosTeclado, int idJugador, Config& configuracion);
	~Vista();

	void iniciar();
	void detener();
	void nuevoEstado(double desplazamiento, std::vector<EstadoObj>& estado);

private:
	void cicloPrincipal();
	void enviarEventos();
	void actualizar();

	bool detenido;
	int idJugador;
	Config& configuracion;

	Teclas teclas;
	ColaBloqueante<int>& eventosTeclado;

	std::mutex lockEstado;
	double desplazamiento;
	std::vector<EstadoObj> estado;

	SDL_Window* ventana;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
};



#endif /* VISTA_H_ */

