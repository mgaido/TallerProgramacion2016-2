#include "CodificadorDeMensajesCliente.h"

CodificadorDeMensajesCliente::CodificadorDeMensajesCliente(Conexion *nuevaCon) {
	con = nuevaCon;
}

CodificadorDeMensajesCliente::~CodificadorDeMensajesCliente() {
}

void CodificadorDeMensajesCliente::enviarMensajeFormateado(std::string destinatario, std::string texto) {
	con->enviar("SND_MESSAGE");
	std::string resp = con->recibir();
	debug(resp);
	if (resp.substr(0, resp.find('-')) == "1") {
		con->enviar("SND_DESTINATARIO");
		resp = con->recibir();
		debug(resp);
		if (resp.substr(0, resp.find('-')) == "1") {
			con->enviar(destinatario);
			resp = con->recibir();
			debug(resp);
			if (resp.substr(0, resp.find('-')) == "1") {
				con->enviar("SND_TEXT");
				resp = con->recibir();
				debug(resp);
				if (resp.substr(0, resp.find('-')) == "1") {
					con->enviar(texto);
					resp = con->recibir();
					if (resp.substr(0, resp.find('-')) == "1") {
						clrScrn();
						info("Mensaje: " +texto+" enviado correctamente a "+destinatario);
						std::cout << "Mensaje enviado correctamente" << std::endl;
						info("Mensaje enviado correctamente");
					}
				}
			}
		}
	}

}


void CodificadorDeMensajesCliente::clrScrn() {
#ifdef _WIN32
	//system("cls");
#endif
}

void CodificadorDeMensajesCliente::recibirMensajes() {
	con->enviar("RCV_MESSAGES");
	std::string resp = con->recibir();
	if (resp.substr(0, resp.find('-')) == "1") {
		int cantidadDeMensajes = std::stoi(resp.substr(resp.find("1-")+2, std::string::npos));
		while (cantidadDeMensajes > 0) {
			con->enviar("DOWNLOAD_MESSAGES");
			resp = con->recibir();
			if (resp.substr(0, resp.find('-')) == "1") {
				resp = resp.substr(resp.find('-')+1);
				std::cout << resp << std::endl << std::endl << std::endl << std::endl;
				cantidadDeMensajes--;
			}
		}
		info("Recepcion de mensajes correcta");
	}
}
