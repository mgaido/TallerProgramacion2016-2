#include "CodificadorDeMensajesServidor.h"

CodificadorDeMensajesServidor::CodificadorDeMensajesServidor(SOCKET socketD) {
	con = new Conexion(socketD);
}

CodificadorDeMensajesServidor::~CodificadorDeMensajesServidor() {
}

void CodificadorDeMensajesServidor::interpretarComando(std::string text) {
	int codeNumber = hashCode(text);
	switch (codeNumber) {
	case SND_MESSAGE: {
		enviarMensaje();
		break;
	}
	case RCV_MESSAGES: {
		devolverMensaje();
		break;
	}

	default:
		con->enviar("0- Comando Invalido");
		break;
	}
}

int CodificadorDeMensajesServidor::hashCode(std::string text) {
	return text.length();
}

void CodificadorDeMensajesServidor::enviarMensaje() {
	Mensaje nuevoMensaje;
	Mensajeria mensajeria;
	con->enviar("1- Exitoso SND_MESSAGE");
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if(codeNumber == SND_DESTINATARIO){
		con->enviar("1- SND_DESTINTARIO");
		resp = con->recibir();
		nuevoMensaje.setDestinatario(*(Usuarios::getUsuario(resp)));
		nuevoMensaje.setRemitente(*usuario);
		con->enviar("1- Destinatario recibido");
		resp = con->recibir();
		codeNumber = hashCode(resp);
		if (codeNumber == SND_TEXT) {
			con->enviar("1- SND_TEXT");
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
	
	con->enviar("1-" + std::to_string(cantidadDeMensajesADevolver));
	
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
	mensajeFormateado += "Destinatario:" + destinatario + CRLF;
	mensajeFormateado += "Remitente: " + remitente + CRLF;
	mensajeFormateado += texto + CRLF;

	return mensajeFormateado;
}

void CodificadorDeMensajesServidor::setUsuario(Usuario *unUsuario) {
	usuario = unUsuario;
}