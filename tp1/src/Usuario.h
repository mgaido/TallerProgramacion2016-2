/*
 * Usuario.h
 *
 *  Created on: 4 sep. 2016
 *      Author: rodrigo
 */

#ifndef USUARIO_H_
#define USUARIO_H_

#include "stdafx.h"

class Usuario {
public:
	Usuario();
	Usuario(std::string &nombre, std::string &password);
	virtual ~Usuario();

	std::string getNombre();
	bool verificarPassword(std::string pass);

	bool operator==(Usuario &otro);
	bool operator!=(Usuario &otro);

private:
	std::string nombre;
	std::string password;
};

#endif /* USUARIO_H_ */
