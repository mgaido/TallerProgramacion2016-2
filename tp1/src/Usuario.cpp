/*
 * Usuario.cpp
 *
 *  Created on: 4 sep. 2016
 *      Author: rodrigo
 */

#include "Usuario.h"

Usuario::Usuario() {
}

Usuario::Usuario(std::string& nombre, std::string& password) {
	this->nombre = nombre;
	this->password = password;
}

std::string Usuario::getNombre() {
	return nombre;
}

bool Usuario::verificarPassword(std::string pass) {
	return this->password == pass;
}

bool Usuario::operator ==(Usuario& otro) {
	return nombre == otro.getNombre();
}

bool Usuario::operator !=(Usuario& otro) {
    return !(*this == otro);
}
