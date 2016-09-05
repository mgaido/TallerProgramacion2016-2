#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "stdafx.h"
#include "Conexion.h"
#include "Mensaje.h"
#include "Usuario.h"

class Servidor {

public:
	Servidor();
	~Servidor();

	void iniciar(int puerto);
	void detener();
private:
	SOCKET socketD;
	bool stop;
	std::thread listenThread;
	std::vector<std::thread> handlers;
	//std::vector <Mensaje> mensajes;
	std::vector <Usuario> usuarios;
	std::string textoUsuarios;

	void Servidor::leerUsuarios();
	void listenLoop(int port);
	void handleClient(SOCKET newSocketD, std::string clientIp);
	bool autentificar(std::string msj);
};

#endif // SERVIDOR_H
