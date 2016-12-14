#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "Config.h"
#include "Handshake.h"
#include "Sesion.h"
#include "Sockets.h"
#include "Logger.h"
#include "Equipo.h"
#include "Utils.h"

class Sesion;

class Servidor {

public:
	Servidor(int puerto, std::string archivo, bool inmortal, char modo);
	~Servidor();

	void iniciar();
	Jugador* nuevaConexion(Sesion* sesion, std::string nombre);
	void recargar();
	void detener();

	Config& getConfiguracion();
	Juego* getJuego();

private:
	SOCKET socketD;
	int puerto;
	std::vector<std::string> archivos;
	bool inmortal;
	char modo;
	std::vector<std::string>::iterator archivo;
	Config config;
	bool detenido;

	std::thread t_aceptarConexiones;
	std::vector<Sesion*> sesiones;
	Juego* juego;
	std::mutex lockJuego;

	std::vector<Equipo> equipos;

	int crearSocket();
	void crearJuego();
	void aceptarConexiones();
	void avanzarJuego();
};

#endif // SERVIDOR_H
