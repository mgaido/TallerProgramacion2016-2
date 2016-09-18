/*
 * Usuario.h
 *
 *  Created on: 4 sep. 2016
 *      Author: rodrigo
 */

#ifndef USUARIO_H_
#define USUARIO_H_

#include "Logger.h"
#include "Utils.h"

class Usuario {
public:
	Usuario();
	Usuario(std::string &nombre, std::string &password);

	std::string getNombre();
	bool verificarPassword(std::string pass);

	bool operator==(Usuario &otro);
	bool operator!=(Usuario &otro);
	std::string password;

private:
	std::string nombre;
};

#endif /* USUARIO_H_ */
