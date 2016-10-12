#ifndef CLIENTE_H
#define CLIENTE_H

#include "Actualizacion.h"
#include "Conexion.h"
#include "Handshake.h"
#include "Logger.h"
#include "Utils.h"
#include "Vista.h"

class Cliente {
public:
	Cliente(std::string host,int puerto, std::string nombre);
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
	bool cerrado;
	std::string usuario;

	ColaBloqueante<int> eventosTeclado;
	Vista* vista;

	std::thread t_enviarEventos;
	std::thread t_recibirAct;
};

#endif // CLIENTE_H
