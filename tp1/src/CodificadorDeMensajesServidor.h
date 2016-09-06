#ifndef CODIFICADOR_DE_MENSAJES_SERVIDOR
#define CODIFICADOR_DE_MENSAJES_SERVIDOR

#include "stdafx.h"
#include "Conexion.h"
#include "Mensaje.h"

#define SND_MESSAGE 11
#define SND_DESTINATARIO 16
#define SND_TEXT 8
#define RCV_MESSAGES 12
#define DOWNLOAD_MESSAGES 17

class CodificadorDeMensajesServidor {
public:
	CodificadorDeMensajesServidor(SOCKET socketD);
	~CodificadorDeMensajesServidor();
	void interpretarComando(std::string);

private:
	Conexion *con;
	int hashCode(std::string text);
	void enviarMensaje(Mensaje &nuevoMensaje);
	void devolverMensaje();
	void preparacionEnviarMensaje();
	void preparacionDevolverMensaje();
	void agregarDestinatarioAMensaje(Mensaje &nuevoMensaje, std::string &destinatario);
	void agregarTextoAMensaje(Mensaje &nuevoMensaje, std::string &texto);
};

#endif
