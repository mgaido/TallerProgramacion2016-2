/*
 * Log.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: rodrigo
 */

#include "Logger.h"
#include <time.h>


std::string nombres[] = {"DEBUG", "INFO", "WARN", "ERROR"};

/* Log */

Log::Log(Nivel nivel, std::string& texto) {
	this->nivel = nivel;
	this->texto = texto;
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	this->fecha = (std::to_string(now->tm_mday) + ":" + std::to_string(now->tm_mon+1) + ":" + std::to_string(now->tm_year+1900));
	this->hora = (std::to_string(now->tm_hour) +":"+ std::to_string(now->tm_min)+":" + std::to_string(now->tm_sec));
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

std::string& Log::getHora() {
	return hora;
}

/* Logger */

Logger::Logger(std::string nombreArchivo) {
	archivo.open(nombreArchivo, std::ios::app);
	if (!archivo.good())
		throw "No se puede escribir en archivo de log";

	ciclo = std::thread(&Logger::desencolar, this);
}

Logger::~Logger() {
	logs.cerrar();
	ciclo.join();
}

void Logger::encolar(Nivel nivel, std::string texto, bool aConsola) {
	Log log(nivel, texto);
	if (! logs.cerrada())
		logs.encolar(log);
	else
		loguear(log);
	if (aConsola) {
		if (nivel == _ERROR)
			std::cerr << texto << std::endl;
		else
			std::cout << texto << std::endl;
	}
}

void Logger::desencolar() {
	while (true) {
		try {
			Log log = logs.desencolar();
			loguear(log);
		} catch (ColaCerrada&) {
			break;
		}
	}
	info("Cerrando Log");
}

void Logger::loguear(Log& log) {
	archivo << log.getFecha() << " " << log.getHora() << " " << log.getNombreNivel();
	archivo << ": " << log.getTexto() << std::endl;
}

