#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "stdafx.h"
#include "Sesion.h"

class Servidor {

public:
	Servidor(int puerto);
	~Servidor();

	void iniciar();
	void detener();
private:
	SOCKET socketD;
	int puerto;
	bool detenido;
	std::thread thread;
	std::vector<Sesion*> sesiones;

	int crearSocket();
	void aceptarConexiones();
};

#endif // SERVIDOR_H
