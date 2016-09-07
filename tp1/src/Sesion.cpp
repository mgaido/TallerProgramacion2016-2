/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip) {
	this->socketD = socketD;
	this->ip = ip;
	this->detenido=false;
	this->logueado = false;
	this->usuario = nullptr;
	this->thread = std::thread(&Sesion::atenderCliente, this);
}

void Sesion::detener() {
	if (! detenido) {
		detenido = true;
		shutdown(socketD, 2);
		thread.join();
	}
}

void Sesion::atenderCliente() {
	Conexion con(socketD);
	CodificadorDeMensajesServidor codificadorDeMensajes(socketD);
	while (! detenido) {
		try {
			if (logueado) {
				std::string text = con.recibir();
				codificadorDeMensajes.interpretarComando(text);
			} else {
				std::string text = con.recibir();
				bool correcto = autentificar(text);
				if (correcto) {
					con.enviar("1-" + Usuarios::getNombres());
					logueado = true;
					codificadorDeMensajes.setUsuario(this->usuario);
					std::cout << "El cliente " << this->ip << " inicio sesion correctamente: " << text << std::endl;
				} else {
					con.enviar("0-Error");
					std::cout << "El cliente " << this->ip << " inicio sesion  incorrectamente: " << text << std::endl;
				}
			}
		} catch (SocketException &e) {
			if (! detenido) {
				//Logger::error(std:string(e.what()));
				std::cout << "Cliente " << ip << " desconectado."<< std::endl;
				detenido = true;
			}
		}
	}
}


bool Sesion::autentificar(std::string msj) {
	if (msj.find(',') != std::string::npos) {
		std::string nombre = msj.substr(0, msj.find(','));
		std::string clave = msj.substr(msj.find(',')+1, msj.length()-1);

		Usuario* usuario = Usuarios::getUsuario(nombre);
		if (usuario != nullptr && usuario->verificarPassword(clave)) {
			this->usuario = usuario;
		}
		else {
			this->usuario = nullptr;
		}
	}
	std::cout << (usuario != nullptr) << std::endl;
	return usuario != nullptr;

}

