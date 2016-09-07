/*
 * Usuarios.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Usuarios.h"

std::vector<Usuario> Usuarios::usuarios;
std::string Usuarios::nombres;

Usuario* Usuarios::getUsuario(std::string nombre) {
	auto it = usuarios.begin();
	while (it != usuarios.end()) {
		if (nombre == it->getNombre()) {
			return &(*it);
		}
		it++;
	}
	return nullptr;
}

std::vector<Usuario> Usuarios::getUsuarios() {
	return usuarios;
}

std::string Usuarios::getNombres(){
	return nombres;
}

bool Usuarios::leerUsuarios(std::string archivo) {
	std::ifstream fileUsuario(archivo);
	if (! fileUsuario.good()){
		error("Error leyendo el archivo " + archivo);
		return false;
	}

	std::string linea;
	std::string clave[2];
	while (fileUsuario.good() && getline(fileUsuario, linea)) {
		bool invalido = false;
		for (int i = 0; i < 2 && !invalido; i++) {
			if (linea.find(',') != std::string::npos || i == 1) {
				clave[i] = linea.substr(0, linea.find(','));
				linea = linea.substr(linea.find(',') + 1, linea.length() - 1);
			}
			else {
				invalido = true;
				warn("Usuario invalido: [" + linea + "]");
			}
		}

		bool repetido = false;
		auto ite = usuarios.begin();
		while (ite != usuarios.end() && !repetido) {
			if (clave[0] == ite->getNombre()) {
				repetido = true;
				warn("Usuario repetido: [" + clave[0] + "]");
			}
			ite++;
		}

		if (!invalido && clave[0] != "" && clave[1] != "" && !repetido) {
			Usuario usuario(clave[0], clave[1]);
			usuarios.push_back(usuario);
			if (nombres.length() > 0)
				nombres = nombres.append(",");
			nombres = nombres.append(clave[0]);
			info("Usuario encontrado: [" + clave[0] + "]");

		} else
			warn("Usuario invalido: [" + linea + "]");
	}

	if (usuarios.size() == 0) {
		error("No se encontraron usuarios en " + archivo);
		return false;
	} else
		return true;
}


