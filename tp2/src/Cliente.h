#ifndef CLIENTE_H
#define CLIENTE_H

#include "Conexion.h"
#include "Logger.h"
#include "Utils.h"
#include "Vista.h"

#include <exception>

class Cliente {
public:
	Cliente(std::string host,int puerto);
	~Cliente();

	void iniciar();
	void desconectar();
private:
	void conectar();
	void enviarEventos();
	void recibirActualizaciones();

	std::string host;
	int puerto;
	Conexion con;
	bool conectado;

	ColaBloqueante<int> eventosTeclado;
	Vista* vista;

	std::thread t_enviarEventos;
	std::thread t_recibirAct;
};

#endif // CLIENTE_H
