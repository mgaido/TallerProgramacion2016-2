#pragma once

#include "ColaBloqueante.h"

#include <string>

using Nivel = int;

Nivel DEBUG = 0;
Nivel INFO = 1;
Nivel WARN = 2;
Nivel ERROR = 3;
Nivel FATAL = 4;

class Log {
	public:
		Nivel nivel;
		std::string texto;
};

class Logger {

public:
	void debug(std::string text);
	void info(std::string text);
	void warn(std::string text);
	void error(std::string text);
	void fatal(std::string text);

private:
	void log(Nivel nivel, std::string texto);
	void logWriter();
	ColaBloqueante<Log> cb;
	
};

void info(std::string texto);