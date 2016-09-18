#ifndef CLIENTE_H
#define CLIENTE_H

#include "Conexion.h"
#include "Logger.h"
#include "Utils.h"

#include <exception>

class EntradaException: public std::exception {
public:
	EntradaException(){};
};

#define RANGO_LONGITUD_MENSAJE_LOREM_IPSUM 200

class Cliente {
public:
	Cliente(std::string host,int puerto);
	~Cliente();

	void iniciar();
private:
	void imprimirMenu();
	char leerComando();
	void conectar();
	void loguear();
	void enviarMensaje();
	void enviarMensaje(int usuario, std::string texto);
	void recibirMensajes();
	void loremIpsum();
	void desconectar();
	void parseoUsuario(std::string textoUsuarios);
	std::string getMensajeLoremIpsum(std::ifstream &archivo, int longitudMensaje);
	void ping();

	std::vector<std::string> enviarComando(std::string msg);
	std::string leerDeConsola();

	std::string host;
	int puerto;
	SOCKET socketD;
	Conexion con;
	bool conectado;
	bool logueado;
	bool detenido;
	std::vector<std::string> usuarios;

	std::mutex conLock;
	std::thread pingThread;
};

#endif // CLIENTE_H
