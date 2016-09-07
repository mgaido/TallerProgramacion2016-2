/*
 * Logger.h
 *
 *  Created on: Sep 6, 2016
 *      Author: rodrigo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "ColaBloqueante.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>


#define DEBUG 1
#define INFO 2
#define WARN 3
#define ERROR 4

#define NIVEL DEBUG

using Nivel = int;

class Log {
public:
	Log(Nivel nivel, std::string &texto, std::string &funcion, std::string &fuente);

	Nivel getNivel();
	std::string& getNombreNivel();
	std::string& getTexto();
	std::string& getFecha();
	std::string& getFuente();
	std::string& getFuncion();
	std::string& getHora();

private:
	Nivel nivel;
	std::string texto;
	std::string funcion;
	std::string fuente;
	std::string fecha;
	std::string hora;
};

class Logger {
public:
	Logger();
	~Logger();

	void encolar(Nivel nivel, std::string texto, std::string funcion, std::string fuente);
	void encolar(Nivel nivel, std::string texto, std::exception &e, std::string funcion, std::string fuente);
private:
	ColaBloqueante<Log> logs;
	std::thread ciclo;
	void desencolar();
	void loguear(Log& log);
};

extern Logger* logger;

#define debug(...) if (NIVEL <= 1) logger->encolar(1, __VA_ARGS__, __func__, __FILE__)
#define info(...) if (NIVEL <= 2) logger->encolar(2, __VA_ARGS__, __func__, __FILE__)
#define warn(...) if (NIVEL <= 3) logger->encolar(3, __VA_ARGS__, __func__, __FILE__)
#define error(...) if (NIVEL <= 4) logger->encolar(4, __VA_ARGS__, __func__, __FILE__)

#endif /* LOGGER_H_ */



