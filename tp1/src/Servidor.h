#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "stdafx.h"
#include "Sesion.h"
#include "Mensajeria.h"
#include "Usuarios.h"

class Servidor {

public:
	Servidor(std::string &archivo);
	~Servidor();

	void iniciar(int puerto);
	void detener();
private:
	SOCKET socketD;
	bool detenido;
	std::thread thread;
	std::vector<Sesion> sesiones;

	void aceptarConexiones(int port);
};

#endif // SERVIDOR_H
