/*
 * Usuarios.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef USUARIOS_H_
#define USUARIOS_H_

#include <string>
#include <vector>

#include "Usuario.h"

class Usuarios {
public:
	static bool leerUsuarios(std::string archivo);
	static Usuario* getUsuario(std::string nombre);
	static std::vector<Usuario> getUsuarios();
	static std::string getNombres();
private:
	static std::vector <Usuario> usuarios;
	static std::string nombres;
};

#endif /* USUARIOS_H_ */
