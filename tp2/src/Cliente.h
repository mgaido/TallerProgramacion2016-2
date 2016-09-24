#ifndef CLIENTE_H
#define CLIENTE_H

#include "Conexion.h"
#include "Logger.h"
#include "Utils.h"

#include <exception>

class Cliente {
public:
	Cliente(std::string host,int puerto);
	~Cliente();

	void iniciar();
private:
	void conectar();
	void desconectar();
	void ping();

	std::vector<std::string> enviarComando(std::string msg);

	std::string host;
	int puerto;
	SOCKET socketD;
	Conexion con;
	bool conectado;
	bool detenido;

	std::mutex conLock;
	std::thread pingThread;
};

#endif // CLIENTE_H
