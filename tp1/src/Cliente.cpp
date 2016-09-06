#include "Cliente.h"

Cliente::Cliente() {
	conectado = false;
	logueado = false;
	socketD = INVALID_SOCKET;
}

void Cliente::conectar(std::string host, int puerto) {
	this->host = host;
	this->puerto = puerto;

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(puerto);

	std::string funcion = "inet_pton";
	int respuesta = inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr.s_addr)) -1;

	if (respuesta >= 0) {
		funcion = "socket";
		respuesta = -1;
		socketD = socket(AF_INET, SOCK_STREAM, 0);

		if (socketD != INVALID_SOCKET) {
			funcion = "connect";
			respuesta = connect(socketD, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

			if (respuesta == 0) {
				std::cout << "Conectado a " << host << ":" << puerto << std::endl;
				conectado = true;
				leerComando();
			}
		}
	}

	if (respuesta < 0) {
		std::string error = "Error llamando a " + funcion + " con código " + std::to_string(getLastError());
		std::cerr << error << std::endl;
	}

}

void Cliente::leerComando() {
	Conexion con(socketD);

	std::string msg;
	std::string resp;

	while (conectado) {
		char opcion = imprimirMenu();
		switch (opcion) {
		case '1':
			while (conectado && !logueado) {
				loguear(con);
			}
			break;
		case '2':
			logueado = false;
			break;
		case '3':
			desconectar();
			break;
		case '4':
			enviarMensaje(con);
			break;
		case '5':
			recibirMensaje(con);
			break;
		case '6':
			loremIpsum(con);
			break;
		default:
			std::cout << "Incorrecto" << std::endl;
			break;
		}
	}
}

char Cliente::imprimirMenu() {
	std::string opcion;
	char opcionChar;
	bool correcto = false;
	while (!correcto) {
		std::cout << "1. Conectar" << std::endl;
		std::cout << "2. Desconectar" << std::endl;
		std::cout << "3. Salir" << std::endl;
		std::cout << "4. Enviar" << std::endl;
		std::cout << "5. Recibir" << std::endl;
		std::cout << "6. Lorem Ipsum" << std::endl;
		std::cout << "Seleccione una opcion: ";
		std::getline(std::cin, opcion);

		if (opcion.length() == 1) {
			opcionChar = opcion[0];
			correcto = true;
		}
		else {
			std::cout << "Opcion Incorrecta" << std::endl;
		}
	}
	return opcionChar;
}

void Cliente::loguear(Conexion con) {
	std::cout << "Ingrese Usuario: ";
	std::string usuario;
	std::getline(std::cin, usuario);
	std::cout << "Ingrese Clave: ";
	std::string clave;
	std::getline(std::cin, clave);

	try {
		con.enviar(usuario + "," + clave);
		std::string resp = con.recibir();

		if (resp.substr(0, resp.find('-')) == "1") {
			logueado = true;
			std::cout << "logueo correcto" << std::endl;
			parseoUsuario(resp.substr(resp.find('-')+1, resp.length()));
		}
		std::cout << "El servidor respondio: " << resp << std::endl;
	} catch (SocketException e) {
		if (conectado) {
			//Logger::error(std:string(e.what()));
			std::cout << "Conexión con " << host << " cerrada."<< std::endl;
			conectado = false;
		}
	}
}

void Cliente::parseoUsuario(std::string textoUsuarios) {
	textoUsuarios = textoUsuarios + ",";
	while (textoUsuarios.find(',') != std::string::npos) {
		usuarios.push_back(textoUsuarios.substr(0, textoUsuarios.find(',')));
		textoUsuarios = textoUsuarios.substr(textoUsuarios.find(',') + 1, textoUsuarios.length()-1);
	}
	//for (std::vector<std::string>::const_iterator i = usuarios.begin(); i != usuarios.end(); ++i) {
	//	// process i
	//	std::cout << *i << " "; // this will print all the contents of *features*
	//}
}

void Cliente::desconectar() {
	if (conectado) {
		conectado = false;
		shutdown(socketD, 2);
	}

}

void Cliente::enviarMensaje(Conexion con){
	CodificadorDeMensajes codificadorDeMensajes(socketD);
	std::cout << "Ingrese destinatario: ";
	std::string destinatario;
	std::getline(std::cin, destinatario);
	std::cout << "Ingrese Mensaje: ";
	std::string texto;
	std::getline(std::cin, texto);
	try {
		codificadorDeMensajes.enviarMensajeFormateado(destinatario, texto);
	}
	catch (SocketException e) {
		if (conectado) {
			//Logger::error(std:string(e.what()));
			std::cout << "Conexión con " << host << " cerrada." << std::endl;
			conectado = false;
		}
	}
}

void Cliente::recibirMensaje(Conexion con) {

}

void Cliente::loremIpsum(Conexion con) {

}