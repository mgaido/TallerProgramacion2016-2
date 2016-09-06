#include "CodificadorDeMensajes.h"

CodificadorDeMensajes::CodificadorDeMensajes(SOCKET socketD){
	con = new Conexion(socketD);
}

void CodificadorDeMensajes::enviarMensajeFormateado(std::string destinatario, std::string texto){


}

CodificadorDeMensajes::~CodificadorDeMensajes() {
	delete con;
}