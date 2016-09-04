#pragma once

#include "Mensaje.h"
#include "Usuario.h"

#include <vector>
#include <algorithm> 
#include <mutex>

class Mensajeria
{
public:
	Mensajeria();
	~Mensajeria();

	void enviarMensaje(std::string &texto, Usuario &remitente, Usuario &destinatario);
	std::vector<Mensaje> getMensajesParaUsuario(Usuario &usuario);
	void insertarMensajes(std::vector<Mensaje> &msgs);
	
private:
	std::vector<Mensaje> mensajes;
	std::mutex lock;
};

