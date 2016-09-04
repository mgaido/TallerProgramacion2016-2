/*
 * Mensaje.cpp
 *
 *  Created on: 4 sep. 2016
 *      Author: rodrigo
 */

#include "Mensaje.h"

Mensaje::Mensaje() {
}

Mensaje::~Mensaje() {
}

const Usuario& Mensaje::getDestinatario() const {
	return destinatario;
}

void Mensaje::setDestinatario(const Usuario& destinatario) {
	this->destinatario = destinatario;
}

const Usuario& Mensaje::getRemitente() const {
	return remitente;
}

void Mensaje::setRemitente(const Usuario& remitente) {
	this->remitente = remitente;
}

std::string Mensaje::getTexto() const {
	return texto;
}

void Mensaje::setTexto(std::string texto) {
	this->texto = texto;
}
