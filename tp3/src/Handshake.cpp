#include "Handshake.h"

HandshakeResponse::HandshakeResponse() {
	aceptado = false;
}

void HandshakeResponse::toBytes(Bytes & bytes) {
	bytes.put(aceptado);
	bytes.put(idJugador);
	bytes.putSerializable(configuracion);
}

void HandshakeResponse::fromBytes(Bytes & bytes) {
	bytes.get(aceptado);
	bytes.get(idJugador);
	bytes.getSerializable(configuracion);
}

bool HandshakeResponse::isAceptado() const {
	return aceptado;
}

void HandshakeResponse::setAceptado(bool aceptado) {
	this->aceptado = aceptado;
}

int HandshakeResponse::getIdJugador() const {
	return idJugador;
}

void HandshakeResponse::setIdJugador(int idJugador) {
	this->idJugador = idJugador;
}


Config HandshakeResponse::getConfiguracion() {
	return configuracion;
}

void HandshakeResponse::setConfiguracion(Config configuracion) {
	this->configuracion = configuracion;
}

std::string HandshakeRequest::getNombre() {
	return std::string(nombre.data());
}

void  HandshakeRequest::setNombre(std::string nombre) {
	nombre.copy(this->nombre.data(), this->nombre.size());
	this->nombre[std::min<int>(nombre.size(), this->nombre.size()-1)] = '\0';
}
