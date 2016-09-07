#include "CodificadorDeMensajesCliente.h"

CodificadorDeMensajesCliente::CodificadorDeMensajesCliente(SOCKET socketD) {
	con = new Conexion(socketD);
}

CodificadorDeMensajesCliente::~CodificadorDeMensajesCliente() {
	delete con;
}

void CodificadorDeMensajesCliente::enviarMensajeFormateado(std::string destinatario, std::string texto) {
	con->enviar("SND_MESSAGE");
	std::string resp = con->recibir();
	std::cout << resp << std::endl;
	if (resp.substr(0, resp.find('-')) == "1") {
		con->enviar("SND_DESTINATARIO");
		resp = con->recibir();
		std::cout << resp << std::endl;
		if (resp.substr(0, resp.find('-')) == "1") {
			con->enviar(destinatario);
			resp = con->recibir();
			std::cout << resp << std::endl;
			if (resp.substr(0, resp.find('-')) == "1") {
				con->enviar("SND_TEXT");
				resp = con->recibir();
				std::cout << resp << std::endl;
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

void CodificadorDeMensajesCliente::recibirMensajes() {
	con->enviar("RCV_MESSAGES");
	std::string resp = con->recibir();
	if (resp.substr(0, resp.find('-')) == "1") {
		int cantidadDeMensajes = std::stoi(resp.substr(resp.find('1-')+1, std::string::npos));
		while (cantidadDeMensajes > 0) {
			con->enviar("DOWNLOAD_MESSAGES");
			resp = con->recibir();
			if (resp.substr(0, resp.find('-')) == "1") {
				resp = resp.substr(resp.find('-')+1);
				std::cout << resp << std::endl << std::endl << std::endl << std::endl;
				cantidadDeMensajes--;
			}
		}
	}
}