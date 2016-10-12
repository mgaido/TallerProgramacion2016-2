#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "Config.h"
#include "Handshake.h"
#include "Sesion.h"
#include "Sockets.h"
#include "Logger.h"
#include "Utils.h"

class Sesion;

struct Peticion {
	SOCKET socketD;
	std::string ip;
};

class Servidor {

public:
	Servidor(int puerto, std::string archivo);
	~Servidor();

	void iniciar();
	void detener();

	Juego* getJuego();
	Config* getConfig();

private:
	SOCKET socketD;
	int puerto;
	Config* config;
	bool detenido;

	Configuracion configuracion;
	std::thread t_aceptarConexiones;
	ColaBloqueante<Peticion> peticiones;
	std::thread t_juego;

	Juego* juego;
	std::thread t_procesarPeticiones;
	std::vector<Sesion*> sesiones;

	int crearSocket();
	void aceptarConexiones();
	void procesarPeticiones();
	void avanzarJuego();
};

#endif // SERVIDOR_H
