/*
 * Log.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: rodrigo
 */

#include "Logger.h"

std::string nombres[] = {"DEBUG", "INFO", "WARN", "ERROR"};

/* Log */

Log::Log(Nivel nivel, std::string& texto, std::string& funcion, std::string& fuente) {
	this->nivel = nivel;
	this->texto = texto;
	this->funcion = funcion;
	this->fuente = fuente;
	this->fecha = __DATE__;
	this->hora = __TIME__;
}

std::string& Log::getTexto() {
	return texto;
}

Nivel Log::getNivel() {
	return nivel;
}

std::string& Log::getNombreNivel() {
	return nombres[nivel-1];
}

std::string& Log::getFecha() {
	return fecha;
}

std::string& Log::getFuente() {
	return fuente;
}

std::string& Log::getFuncion() {
	return funcion;
}

std::string& Log::getHora() {
	return hora;
}

/* Logger */

Logger::Logger() {
	ciclo = std::thread(&Logger::desencolar, this);
}

Logger::~Logger() {
	logs.cerrar();
	ciclo.join();
}

void Logger::encolar(Nivel nivel, std::string texto, std::string funcion, std::string fuente) {
	Log log(nivel, texto, funcion, fuente);
	if (! logs.cerrada())
		logs.encolar(log);
	else
		loguear(log);
}

void Logger::encolar(Nivel nivel, std::string texto, std::exception& e, std::string funcion, std::string fuente) {
	texto = texto + " " + e.what();
	Log log(nivel, texto, funcion, fuente);
	logs.encolar(log);
}

void Logger::desencolar() {
	while (true) {
		try {
			Log log = logs.desencolar();
			loguear(log);
		} catch (ColaCerrada) {
			break;
		}
	}
	info("Cerrando Log");
}

void Logger::loguear(Log& log) {
	std::cout << log.getFecha() << " " << log.getHora() << " " << log.getNombreNivel() << " ["
			<< log.getFuente() << " " << log.getFuncion() << "]: " << log.getTexto() << std::endl;
}

