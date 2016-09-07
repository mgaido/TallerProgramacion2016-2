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

Usuario& Mensaje::getDestinatario() {
	return destinatario;
}

void Mensaje::setDestinatario(Usuario& destinatario) {
	this->destinatario = destinatario;
}

Usuario& Mensaje::getRemitente() {
	return remitente;
}

void Mensaje::setRemitente(Usuario& remitente) {
	this->remitente = remitente;
}

std::string& Mensaje::getTexto() {
	return texto;
}

void Mensaje::setTexto(std::string& texto) {
	this->texto = texto;
}
