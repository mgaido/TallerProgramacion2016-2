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
	Jugador* nuevaConexion(Sesion* sesion, std::string nombre);
	void recargar();
	void detener();

	Config& getConfiguracion();
	Juego* getJuego();

private:
	SOCKET socketD;
	int puerto;
	std::string archivo;
	Config config;
	bool detenido;

	std::thread t_aceptarConexiones;
	std::vector<Sesion*> sesiones;
	std::thread t_juego;
	Juego* juego;
	std::mutex lockJuego;

	int crearSocket();
	void crearJuego();
	void aceptarConexiones();
	void avanzarJuego();
};

#endif // SERVIDOR_H
