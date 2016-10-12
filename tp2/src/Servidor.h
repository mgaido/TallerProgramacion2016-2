#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "Config.h"
#include "Handshake.h"
#include "Sesion.h"
#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

class Sesion;

class Servidor {

public:
	Servidor(int puerto, std::string archivo);
	~Servidor();

	void iniciar();
	void detener();

	void removerSesion(Sesion* sesion);
	Juego* getJuego();
	Config* getConfig();

private:
	SOCKET socketD;
	int puerto;
	Config* config;
	bool detenido;
	std::thread t_aceptarConexiones;
	std::vector<Sesion*> sesiones;
	Juego* juego;
	std::thread t_juego;
	int crearSocket();
	void aceptarConexiones();
	void avanzarJuego();
};

#endif // SERVIDOR_H
