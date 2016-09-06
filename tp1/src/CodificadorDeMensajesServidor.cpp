#include "CodificadorDeMensajesServidor.h"

CodificadorDeMensajesServidor::CodificadorDeMensajesServidor(SOCKET socketD) {
	con = new Conexion(socketD);
}

CodificadorDeMensajesServidor::~CodificadorDeMensajesServidor() {
	delete con;
}

void CodificadorDeMensajesServidor::interpretarComando(std::string text) {
	int codeNumber = hashCode(text);
	switch (codeNumber) {
	case SND_MESSAGE: {
		preparacionEnviarMensaje();
	}
	case RCV_MESSAGES: {
		preparacionDevolverMensaje();
	}

	default:
		con->enviar("0- Comando Invalido");
	}
}

int CodificadorDeMensajesServidor::hashCode(std::string text) {
	return text.length();
}

void CodificadorDeMensajesServidor::preparacionEnviarMensaje() {
	Mensaje nuevoMensaje;
	con->enviar("1- Exitoso");
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if(codeNumber == SND_DESTINATARIO){
		con->enviar("1- Exitoso");
		resp = con->recibir();
		agregarDestinatarioAMensaje(nuevoMensaje, resp);
		resp = con->recibir();
		codeNumber = hashCode(resp);
		if (codeNumber == SND_TEXT) {
			resp = con->recibir();
			agregarTextoAMensaje(nuevoMensaje, resp);
			enviarMensaje(nuevoMensaje);
			con->enviar("1- Mensaje enviado");
			
		} else {
			con->enviar("0- Comando Invalido");
		}

	} else {
		con->enviar("0- Comando Invalido");
	}
}

void CodificadorDeMensajesServidor::preparacionDevolverMensaje() {
	int cantidadDeMensajesADevolver;
	con->enviar("1-"+cantidadDeMensajesADevolver);
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if (codeNumber == DOWNLOAD_MESSAGES) {
		
	} else {
		con->enviar("0- Comando Invalido");
	}
}

void CodificadorDeMensajesServidor::agregarDestinatarioAMensaje(Mensaje &nuevoMensaje, std::string &destinatario) {

}

void CodificadorDeMensajesServidor::agregarTextoAMensaje(Mensaje &nuevoMensaje, std::string &texto) {

}

void CodificadorDeMensajesServidor::enviarMensaje(Mensaje &nuevoMensaje) {

}

void CodificadorDeMensajesServidor::devolverMensaje() {

}