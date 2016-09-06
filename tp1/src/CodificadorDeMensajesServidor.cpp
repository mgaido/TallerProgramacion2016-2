#include "CodificadorDeMensajesServidor.h"

CodificadorDeMensajesServidor::CodificadorDeMensajesServidor(SOCKET socketD, Usuario *usuario) {
	con = new Conexion(socketD);
}

CodificadorDeMensajesServidor::~CodificadorDeMensajesServidor() {
	delete con;
}

void CodificadorDeMensajesServidor::interpretarComando(std::string text) {
	int codeNumber = hashCode(text);
	switch (codeNumber) {
	case SND_MESSAGE: {
		enviarMensaje();
	}
	case RCV_MESSAGES: {
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
	Mensaje nuevoMensaje;
	Mensajeria mensajeria;
	con->enviar("1- Exitoso");
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if(codeNumber == SND_DESTINATARIO){
		con->enviar("1- Exitoso");
		resp = con->recibir();
		nuevoMensaje.setDestinatario(*Usuarios::getUsuario(resp));
		nuevoMensaje.setRemitente(*usuario);
		resp = con->recibir();
		codeNumber = hashCode(resp);
		if (codeNumber == SND_TEXT) {
			resp = con->recibir();
			nuevoMensaje.setTexto(resp);
			mensajeria.enviarMensaje(nuevoMensaje.getTexto(), nuevoMensaje.getRemitente(), nuevoMensaje.getDestinatario());
			con->enviar("1- Mensaje enviado");
		} else {
			con->enviar("0- Comando Invalido");
		}

	} else {
		con->enviar("0- Comando Invalido");
	}
}

void CodificadorDeMensajesServidor::devolverMensaje() {
	Mensajeria mensajeria;
	std::vector<Mensaje> mensajesADevolver = mensajeria.getMensajesParaUsuario(*usuario);
	int cantidadDeMensajesADevolver= mensajesADevolver.size();

	con->enviar("1-"+cantidadDeMensajesADevolver);
	
	auto iterador = mensajesADevolver.begin();
	while (iterador != mensajesADevolver.end()) {
		std::string resp = con->recibir();
	
		int codeNumber = hashCode(resp);
		if (codeNumber == DOWNLOAD_MESSAGES) {
			con->enviar("1-" + formatearMensaje(iterador->getDestinatario().getNombre(), iterador->getRemitente().getNombre(), iterador->getTexto()));
			iterador++;
		} else {
			con->enviar("0- Comando Invalido");
		}
	}
}

std::string CodificadorDeMensajesServidor::formatearMensaje(std::string destinatario, std::string remitente, std::string texto) {
	std::string mensajeFormateado = "";
	std::string const CRLF = "\r\n";
	mensajeFormateado += destinatario + CRLF;
	mensajeFormateado += remitente + CRLF;
	mensajeFormateado += texto + CRLF;

	return mensajeFormateado;
}