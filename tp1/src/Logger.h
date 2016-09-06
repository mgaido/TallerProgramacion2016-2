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
#include <iostream>
#include <fstream>
#include <thread>

#define NIVEL = 1;

using Nivel = int;

class Log {
public:
	Log(Nivel nivel, std::string &texto);
	std::string getTexto();
	Nivel getNivel();
private:
	std::string texto;
	Nivel nivel;
};

class Logger {
public:
	Logger();
	~Logger();

	void debug(std::string texto);
	void info(std::string texto);
	void warn(std::string texto);
	void error(std::string texto);
	void error(std::string texto, std::exception &e);
private:
	ColaBloqueante<Log> logs;
	std::thread ciclo;

	void encolar(Nivel nivel, std::string texto);
	void loguear();
};

extern Logger logger;

#endif /* LOGGER_H_ */



