#include "CodificadorDeMensajes.h"

CodificadorDeMensajes::CodificadorDeMensajes(SOCKET socketD) {
	con = new Conexion(socketD);
}

CodificadorDeMensajes::~CodificadorDeMensajes() {
	delete con;
}

void CodificadorDeMensajes::enviarMensajeFormateado(std::string destinatario, std::string texto) {
	con->enviar("SND_MESSAGE");
	std::string resp = con->recibir();
	if (resp.substr(0, resp.find('-')) == "1") {
		con->enviar("SND_DESTINATARIO");
		resp = con->recibir();
		if (resp.substr(0, resp.find('-')) == "1") {
			con->enviar(destinatario);
			resp = con->recibir();
			if (resp.substr(0, resp.find('-')) == "1") {
				con->enviar("SND_TEXT");
				resp = con->recibir();
				if (resp.substr(0, resp.find('-')) == "1") {
					con->enviar(texto);
					resp = con->recibir();
					if (resp.substr(0, resp.find('-')) == "1") {
						std::cout << "Mensaje enviado correctamente" << std::endl;
					}
				}
			}
		}
	}

}

void CodificadorDeMensajes::recibirMensajes() {
	con->enviar("RCV_MESSAGES");
	std::string resp = con->recibir();
	if (resp.substr(0, resp.find('-')) == "1") {
		int cantidadDeMensajes = std::stoi(resp.substr(resp.find('-1'), 1));
		while (cantidadDeMensajes > 0) {
			con->enviar("DOWNLOAD_MESSAGES");
			resp = con->recibir();
			if (resp.substr(0, resp.find('-')) == "1") {
				resp = resp.substr(resp.find('-'));
				parsearMensaje(resp);
				cantidadDeMensajes--;
			}
		}
	}
}

void CodificadorDeMensajes::parsearMensaje(std::string mensaje) {

}