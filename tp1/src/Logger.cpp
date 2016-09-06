/*
 * Log.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: rodrigo
 */

#include "Logger.h"

Nivel DEBUG = 1;
Nivel INFO = 2;
Nivel WARN = 3;
Nivel ERROR = 4;

/* Log */

Log::Log(Nivel nivel, std::string& texto) {
	this->nivel = nivel;
	this->texto = texto;
}

std::string Log::getTexto() {
	return texto;
}

Nivel Log::getNivel() {
	return nivel;
}

/* Logger */

Logger::Logger() {
	ciclo = std::thread(&Logger::loguear, this);
}

Logger::~Logger() {
	logs.cerrar();
	ciclo.join();
}

void Logger::encolar(Nivel nivel, std::string texto) {
	Log log(nivel, texto);
	logs.encolar(log);
}

void Logger::loguear() {
	while (true) {
		try {
			Log log = logs.desencolar();

			std::cout << log.getTexto() << std::endl;
		} catch (ColaCerrada) {
			break;
		}
	}
}

void Logger::debug(std::string texto) {
	encolar(DEBUG, texto);
}

void Logger::info(std::string texto) {
	encolar(INFO, texto);
}

void Logger::warn(std::string texto) {
	encolar(WARN, texto);
}

void Logger::error(std::string texto) {
	encolar(ERROR, texto);
}

void Logger::error(std::string texto, std::exception& e) {
	std::string msg = texto + " Causado por: " + e.what();
	encolar(ERROR, texto);
}

