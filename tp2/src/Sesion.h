/*
 * Sesion.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef SESION_H_
#define SESION_H_

#include "Conexion.h"
#include "Teclas.h"
#include "Juego.h"
#include "Servidor.h"

class Servidor;

class Sesion {
public:
	Sesion(SOCKET socketD, std::string ip, Servidor* servidor);
	~Sesion();
	void nuevaActualizacion(Bytes bytes);
	void detener();

private:
	void atenderCliente();
	void eventoTeclado(Bytes& bytes);
	void desconectar();

	void enviarActializaciones();

	bool detenido;
	Servidor* servidor;
	Jugador* jugador;

	std::string ip;
	Conexion con;
	ColaBloqueante<Bytes> actualizaciones;

	std::thread t_atenderCliente;
	std::thread t_enviarActualizaciones;
};

#endif /* SESION_H_ */
