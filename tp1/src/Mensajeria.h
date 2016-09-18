#pragma once

#include "Mensaje.h"
#include "Usuario.h"
#include "Logger.h"

class Mensajeria
{
public:
	static void enviarMensaje(std::string &texto, Usuario &remitente, Usuario &destinatario);
	static std::vector<Mensaje> getMensajesParaUsuario(Usuario &usuario);
	static void insertarMensajes(std::vector<Mensaje> &msgs);
	
private:
	static std::vector<Mensaje> mensajes;
	static std::mutex lock;
};

