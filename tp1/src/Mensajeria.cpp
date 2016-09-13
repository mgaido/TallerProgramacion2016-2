#include "Mensajeria.h"

std::vector<Mensaje> Mensajeria::mensajes;
std::mutex Mensajeria::lock;

void Mensajeria::enviarMensaje(std::string & texto, Usuario & remitente, Usuario & destinatario) {
	Mensaje mensaje;
	mensaje.setDestinatario(destinatario);
	mensaje.setRemitente(remitente);
	mensaje.setTexto(texto);
	lock.lock();
	mensajes.push_back(mensaje);
	lock.unlock();
	info("Mensaje enviado de " + remitente.getNombre() + " a " + destinatario.getNombre() + ": " + texto);
}

std::vector<Mensaje> Mensajeria::getMensajesParaUsuario(Usuario &usuario) {
	std::vector<Mensaje> msgs;
	lock.lock();
	auto it = mensajes.begin();
	
	while (it != mensajes.end()) {
		if (it->getDestinatario() == usuario) {
			msgs.push_back(*it);
			it = mensajes.erase(it);
		} else {
			it++;
		}
	}
	lock.unlock();
	return msgs;
}

void Mensajeria::insertarMensajes(std::vector<Mensaje>& msgs) {
	mensajes.insert(mensajes.begin(), msgs.begin(), msgs.end());
	msgs.clear();
}


