#include "CodificadorDeMensajesServidor.h"

CodificadorDeMensajesServidor::CodificadorDeMensajesServidor(Conexion *nuevaCon) {
	con = nuevaCon;
	detenido = false;
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

	case DISCONNECT: {
		desconectar();
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
	con->enviar("1- Exitoso SND_MESSAGE");
	std::string resp = con->recibir();
	int codeNumber = hashCode(resp);
	if(codeNumber == SND_DESTINATARIO){
		con->enviar("1- Exitoso SND_DESTINTARIO");
		resp = con->recibir();

		int index = -1;
		try {
			index = std::stoi(resp)-1;
		} catch (std::invalid_argument){}

		std::vector<Usuario> usuarios = Usuarios::getUsuarios();
		if (index < 0 || (unsigned) index > usuarios.size()){
			con->enviar("0- Usuario Invalido");
			return;
		}

		std::vector<Usuario> dest;
		if (index == usuarios.size()){
			dest.insert(dest.begin(), usuarios.begin(), usuarios.end());
		} else {
			dest.push_back(usuarios[index]);
		}

		con->enviar("1- Destinatario recibido");
		resp = con->recibir();

		codeNumber = hashCode(resp);
		if (codeNumber == SND_TEXT) {
			con->enviar("1- SND_TEXT");
			resp = con->recibir();

			auto it = dest.begin();
			while( it != dest.end()) {
				Mensajeria::enviarMensaje(resp, *usuario, *it);
				it++;
			}
			con->enviar("1- Mensaje enviado");
		} else {
			con->enviar("0- Comando Invalido");
		}

	} else {
		con->enviar("0- Comando Invalido");
	}
}

void CodificadorDeMensajesServidor::devolverMensaje() {
	std::vector<Mensaje> mensajesADevolver = Mensajeria::getMensajesParaUsuario(*usuario);
	int cantidadDeMensajesADevolver= mensajesADevolver.size();
	if (cantidadDeMensajesADevolver == 0) {
		con->enviar("0- Comando Invalido");		
	}
	else {
		con->enviar("1-" + std::to_string(cantidadDeMensajesADevolver));
		auto iterador = mensajesADevolver.begin();
		while (iterador != mensajesADevolver.end()) {
			std::string resp = con->recibir();
			int codeNumber = hashCode(resp);
			if (codeNumber == DOWNLOAD_MESSAGES) {
				con->enviar("1-" + formatearMensaje(iterador->getDestinatario().getNombre(), iterador->getRemitente().getNombre(), iterador->getTexto()));
				iterador++;
			}
			else {
				con->enviar("0- Comando Invalido");
				//iterador++;
			}
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

bool CodificadorDeMensajesServidor::pedidoDeDesconexion() {
	return detenido;
}

void CodificadorDeMensajesServidor::desconectar() {
	detenido = true;
	con->enviar("1- Desconexion exitosa");
}
