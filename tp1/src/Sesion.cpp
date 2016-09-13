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
	info("Cliente " + ip + " conectado. Esperando autentificacion.");

	con = Conexion(socketD);
	while (!detenido) {
		try {
			std::string text = con.recibir();
			std::vector<std::string> parametros = split(text, DELIM);

			if (! parametros.empty()) {
				std::string comando = parametros[0];
				parametros.erase(parametros.begin());

				if (comando == AUTH) {
					autentificar(parametros);
				} else if (comando == SEND) {
					enviarMensaje(parametros);
				} else if (comando == RETR) {
					devolverMensajes();
				} else if (comando == BYE) {
					desconectar();
				} else {
					warn("Comando inválido: " + comando);
					std::stringstream ss;
					ss << ERROR << DELIM << "Comando inválido: " + comando;
					con.enviar(ss.str());
				}
			}
		}
		catch (SocketException &e) {
			if (!detenido) {
				error("Error de conexión con " + ip);
				detenido = true;
			}
		}
	}
}

void Sesion::autentificar(std::vector<std::string> parametros) {
	std::string errorMsg;

	if (parametros.size() == 2) {
		std::string nombre = parametros[0];
		std::string clave = parametros[1];

		Usuario* usuario = Usuarios::getUsuario(nombre);
		if (usuario != nullptr && usuario->verificarPassword(clave)) {
			this->usuario = usuario;
			logueado = true;
			std::stringstream ss;
			ss << SUCCESS << DELIM << Usuarios::getNombres();
			con.enviar(ss.str());
			info("El cliente " + this->ip + " inicio sesion correctamente con el usuario " + usuario->getNombre());
			return;
		} else {
			errorMsg = "Usuario o password incorrectos";
			this->usuario = nullptr;
		}
	} else
		errorMsg = "Cantidad de parametros incorrecta";

	warn("El cliente " + this->ip + " inicio sesion incorrectamente: " + errorMsg);
	std::stringstream ss;
	ss << ERROR << DELIM << errorMsg;
	con.enviar(ss.str());
}

void Sesion::enviarMensaje(std::vector<std::string> parametros) {
	std::string errorMsg;

	if (parametros.size() == 2) {
		int index = -1;
		try {
			index = std::stoi(parametros[0]);
		} catch (std::invalid_argument) {}

		std::vector<Usuario> usuarios = Usuarios::getUsuarios();
		if (index >= 0 || (unsigned) index <= usuarios.size()) {
			std::vector<Usuario> dest;
			if ((unsigned) index == usuarios.size()) {
				dest.insert(dest.begin(), usuarios.begin(), usuarios.end());
			} else {
				dest.push_back(usuarios[index]);
			}

			auto it = dest.begin();
			while (it != dest.end()) {
				Mensajeria::enviarMensaje(parametros[1], *usuario, *it);
				it++;
			}
			con.enviar(SUCCESS);
			return;
		} else {
			errorMsg = std::to_string(index) + " no es un indice valido.";
		}
	} else
		errorMsg = "Cantidad de parametros incorrecta.";

	warn(errorMsg);

	std::stringstream ss;
	ss << ERROR << DELIM << errorMsg;
	con.enviar(ss.str());
}

void Sesion::devolverMensajes() {
	std::vector<Mensaje> mensajes = Mensajeria::getMensajesParaUsuario(*usuario);

	std::stringstream ss;
	ss << SUCCESS;

	auto it = mensajes.begin();
	while (it != mensajes.end()) {
		ss << DELIM << it->getRemitente().getNombre()
			<< DELIM << it->getDestinatario().getNombre()
			<< DELIM << it->getTexto();
		it++;
	}
	try {
		con.enviar(ss.str());
	} catch (SocketException e) {
		error("Error devolviendo mensajes. Se reinsertan en la lista");
		Mensajeria::insertarMensajes(mensajes);
	}
}

void Sesion::desconectar() {
	detenido = true;
	con.enviar(BYE);
	info("Cliente " + ip + " desconectado.");
}
