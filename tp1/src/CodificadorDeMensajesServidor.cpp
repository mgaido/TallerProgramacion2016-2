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
		con->enviar("1- Exitoso");
		enviarMensaje();
	}
	case RCV_MESSAGES: {
		con->enviar("1- Exitoso");
		devolverMensaje();
	}

	default:
		con->enviar("0- Comando Invalido");
	}
}

int CodificadorDeMensajesServidor::hashCode(std::string text) {
	return text.length();
}

void CodificadorDeMensajesServidor::enviarMensaje() {
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if(codeNumber == SND_DESTINATARIO){
		con->enviar("1- Exitoso");
		resp = con->recibir();
		codeNumber = hashCode(resp);
		if (codeNumber == SND_TEXT) {
			con->enviar("1- Mensaje enviado");
			
		} else {
			con->enviar("0- Comando Invalido");
		}

	} else {
		con->enviar("0- Comando Invalido");
	}
}

void CodificadorDeMensajesServidor::devolverMensaje() {
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if (codeNumber == DOWNLOAD_MESSAGES) {
		
	} else {
		con->enviar("0- Comando Invalido");
	}
}

