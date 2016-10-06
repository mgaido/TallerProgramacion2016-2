/*
 * Logger.h
 *
 *  Created on: Sep 6, 2016
 *      Author: rodrigo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "ColaBloqueante.h"
#include "Utils.h"

#define _DEBUG 1
#define _INFO 2
#define _WARN 3
#define _ERROR 4

#define NIVEL _DEBUG

using Nivel = int;

class Log {
public:
	Log(Nivel nivel, std::string &texto);

	Nivel getNivel();
	std::string& getNombreNivel();
	std::string& getTexto();
	std::string& getFecha();
	std::string& getHora();

private:
	Nivel nivel;
	std::string texto;
	std::string fecha;
	std::string hora;
};

class Logger {
public:
	Logger(std::string nombreArchivo);
	~Logger();

	void encolar(Nivel nivel, std::string texto, bool aConsola=false);
private:
	ColaBloqueante<Log> logs;
	std::ofstream archivo;
	std::thread ciclo;

	void desencolar();
	void loguear(Log& log);
};

extern Logger* logger;

#define debug(...) if (NIVEL <= _DEBUG) logger->encolar(_DEBUG, __VA_ARGS__)
#define info(...) if (NIVEL <= _INFO) logger->encolar(_INFO, __VA_ARGS__)
#define warn(...) if (NIVEL <= _WARN) logger->encolar(_WARN, __VA_ARGS__)
#define error(...) if (NIVEL <= _ERROR) logger->encolar(_ERROR, __VA_ARGS__)

#endif /* LOGGER_H_ */



