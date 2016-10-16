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

class Sesion {
public:
	Sesion(SOCKET socketD, std::string ip, Jugador* jugador);
	~Sesion();

	bool estaActiva();
	void activar(SOCKET socketD);
	std::string getNombre();

	Jugador* getJugador();
	void cambioDeEstado(Bytes bytes);
	void desconectar();

private:
	void atenderCliente();
	void eventoTeclado(Bytes& bytes);
	void enviarEstado();
	void enviarConfiguraciones();

	bool activa;
	Jugador* jugador;
	ColaBloqueante<Bytes> estados;

	std::string ip;
	Conexion con;

	std::thread t_atenderCliente;
	std::thread t_enviarEstado;
};

#endif /* SESION_H_ */
