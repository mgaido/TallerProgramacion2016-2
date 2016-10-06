/*
 * Sesion.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef SESION_H_
#define SESION_H_

#include "Conexion.h"
#include "Juego.h"
#include "Logger.h"
#include "Teclas.h"
#include "Utils.h"

class Sesion {
public:
	Sesion(SOCKET socketD, std::string ip, Juego* juego);
	~Sesion();
	void nuevaActualizacion(Bytes bytes);
	void detener();

private:
	void atenderCliente();
	void eventoTeclado(Bytes& bytes);
	void desconectar();

	void enviarActializaciones();

	bool detenido;
	Juego* juego;
	Jugador* jugador;

	std::string ip;
	Conexion con;
	ColaBloqueante<Bytes> actualizaciones;

	std::thread t_events;
	std::thread t_updates;
};

#endif /* SESION_H_ */
