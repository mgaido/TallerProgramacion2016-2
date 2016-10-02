/*
 * Sesion.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef SESION_H_
#define SESION_H_

#include "Conexion.h"
#include "Logger.h"
#include "Teclas.h"
#include "Utils.h"

class Sesion {
public:
	Sesion(SOCKET socketD, std::string ip);
	~Sesion();
	void detener();
private:
	void atenderCliente(SOCKET socketD);
	void eventoTeclado(Teclas& teclas);
	void desconectar();

	bool detenido;
	std::string ip;
	Conexion con;
	std::thread thread;
};

#endif /* SESION_H_ */
