/*
 * Sesion.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "Sesion.h"

Sesion::Sesion(SOCKET socketD, std::string ip, Servidor* servidor) {
	this->ip = ip;
	this->activa=true;
	this->enJuego=false;
	this->servidor = servidor;
	this->jugador = nullptr;

	con.setSocket(socketD);
	this->t_atenderCliente = std::thread(&Sesion::atenderCliente, this);
	this->t_enviarEstado = std::thread(&Sesion::enviarEstado, this);
}

Sesion::~Sesion(){
}

bool Sesion::estaActiva() {
	return activa;
}

Jugador* Sesion::getJugador() {
	return jugador;
}

std::string Sesion::getNombre() {
	return nombre;
}

void Sesion::cambioDeEstado(Bytes bytes) {
	if (activa)
		estados.encolar(bytes);
}

void Sesion::atenderCliente() {
	while (activa) {
		try {
			Bytes bytes = con.recibir();
			if (bytes.size() > 0) {
				int comando;
				bytes.get(comando);
				if (comando == HSK_REQ) {
					handshake(bytes);
				} else if (enJuego && comando == KEY) {
					eventoTeclado(bytes);
				} else if (comando == BYE) {
					info("Jugador '" + jugador->getNombre()	+ "' desconectado desde " + ip, true);
					desconectar();
				} else {
					warn("Comando invalido: " + std::to_string(comando));
				}
			}
		} catch (SocketException&) {
			if (con.getPuedeRecibir()) {
				error("Error de conexion con " + ip);
				desconectar();
			}
		}
	}
}

void Sesion::handshake(Bytes& bytes) {
	HandshakeRequest req;
	bytes.get(req);
	nombre = req.getNombre();
	debug("Cliente '" + nombre + "' conectado desde " + ip);

	jugador = servidor->nuevaConexion(this, nombre);

	HandshakeResponse res;

	if (jugador != nullptr) {
		res.setAceptado(true);
		res.setIdJugador(jugador->getId());
		res.setConfiguracion(servidor->getConfiguracion());
		jugador->setConectado(servidor->getJuego()->estaIniciado());
	} else
		res.setAceptado(false);

	activa = res.isAceptado();
	bytes = Bytes();
	bytes.put(HSK_RES);
	bytes.putSerializable(res);
	con.enviar(bytes);

	debug("Respuesta a handshake enviada");
	enJuego = activa;
}

void Sesion::eventoTeclado(Bytes& bytes) {
	int estado=0;
	bytes.get(estado);
	Teclas teclas;
	teclas.setEstado(estado);

	if (teclas.recargar()) {
		servidor->recargar();
		return;
	}

	if (teclas.arriba()) {
		jugador->saltar();
		debug(ip + " tecla arriba");
	}  else if (teclas.der() && !teclas.izq()) {
		jugador->caminar(Direccion::DERECHA);
		debug(ip + " tecla derecha");
	} else if (teclas.izq() && !teclas.der()) {
		jugador->caminar(Direccion::IZQUIERDA);
		debug(ip + " tecla izquierda");
	} else if (teclas.disparar()) {
		Proyectil* nuevoProyectil = jugador->disparar();
		if(nuevoProyectil != NULL)
			servidor->enviarProyectilAJuego(nuevoProyectil);
		debug(ip + " disparar ");
	} else {
		jugador->detenerse();
		debug(ip + " sin teclas");
	}
}

void Sesion::enviarEstado() {
	while (activa) {
		if (! enJuego) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		try {
			Bytes bytes = estados.desencolar();
			con.enviar(bytes);
		} catch (ColaCerrada&) {
			break;
		} catch (SocketException&) {
			desconectar();
			break;
		}
	}
}

void Sesion::recargar() {
	Bytes bytes;
	bytes.put(RLD);
	try {
		con.enviar(bytes);
	} catch (SocketException&) {
		desconectar();
	}
	jugador->setConectado(false);
	enJuego = false;
}

void Sesion::desconectar() {
	if (activa) {
		activa = false;
		estados.cerrar();
		con.cancelarRecepcion();
		jugador->setConectado(false);

		Bytes bytes;
		bytes.put(BYE);
		try {
			con.enviar(bytes);
		} catch (SocketException&) {}
	}

	if (t_enviarEstado.joinable() && t_enviarEstado.get_id() != std::this_thread::get_id()) {
		debug("Esperando que termine thread enviarEstado");
		t_enviarEstado.join();
		debug("Thread enviarEstado termino");
	}

	if (t_atenderCliente.joinable() && t_atenderCliente.get_id() != std::this_thread::get_id()) {
		debug("Esperando que termine thread atenderCliente");
		t_atenderCliente.join();
		debug("Thread atenderCliente termino");
	}

	con.cerrar();
}
