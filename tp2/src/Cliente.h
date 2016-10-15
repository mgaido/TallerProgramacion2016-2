#ifndef CLIENTE_H
#define CLIENTE_H

#include "Conexion.h"
#include "Handshake.h"
#include "Logger.h"
#include "Utils.h"
#include "Vista.h"
#include "Config.h"
#include "Estado.h"

class Cliente {
public:
	Cliente(std::string host,int puerto, std::string nombre);
	~Cliente();

	void iniciar();
	void desconectar();
private:
	void conectar();
	void enviarEventos();
	void recibirEstado();

	std::string host;
	int puerto;
	Conexion con;
	bool conectado;
	bool cerrado;
	std::string usuario;

	ColaBloqueante<int> eventosTeclado;
	Vista* vista;
	Config config;

	std::thread t_enviarEventos;
	std::thread t_recibirEstado;
};

#endif // CLIENTE_H
