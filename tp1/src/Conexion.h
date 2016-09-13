#ifndef CONEXION_H
#define CONEXION_H

#include "stdafx.h"
#include "SocketException.h"

#define DELIM '\0'

#define AUTH "AUTH"
#define SEND "SEND"
#define RETR "RETR"
#define BYE "BYE"

#define SUCCESS "SUCCESS"
#define ERROR "ERROR"

class Conexion {
public:
	Conexion(SOCKET socketD);
	Conexion();

	void enviar(std::string mensaje);
	std::string recibir();
private:
	SOCKET socketD;
};


inline std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> parts;

	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (getline(ss, item, delim)) {
		parts.push_back(item);
	}
	return parts;
}

#endif // CONEXION_H

