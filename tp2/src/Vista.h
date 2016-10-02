/*
 * Vista.h
 *
 *  Created on: Sep 26, 2016
 *      Author: rodrigo
 */

#ifndef VISTA_H_
#define VISTA_H_

#include "Conexion.h"
#include "Teclas.h"
#include "Logger.h"

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

private:

	void leerEventos();

	//void recibirActualizaciones();
	//void dibujar();

	bool detenido;
	ColaBloqueante<int>& eventosTeclado;
	SDL_Window* ventana;
};



#endif /* VISTA_H_ */
