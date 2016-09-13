/*
 * Sesion.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef SESION_H_
#define SESION_H_

#include "Usuarios.h"
#include "Usuario.h"
#include "Mensajeria.h"
#include "Mensaje.h"
#include "Conexion.h"

class Sesion {
public:
	Sesion(SOCKET socketD, std::string ip);
	void detener();
private:
	void atenderCliente();

	void autentificar(std::vector<std::string> parametros);
	void enviarMensaje(std::vector<std::string> parametros);
	void devolverMensajes();
	void desconectar();

	bool detenido;
	std::string ip;
	SOCKET socketD;
	Conexion con;
	std::thread thread;

	bool logueado;
	Usuario* usuario;
};

#endif /* SESION_H_ */
