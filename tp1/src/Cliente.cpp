#include "Cliente.h"

Cliente::Cliente(std::string cHost, int cPuerto) {
	conectado = false;
	logueado = false;
	socketD = INVALID_SOCKET;
	puerto = cPuerto;
	host = cHost;
}

void Cliente::conectar() {

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(puerto);

	std::string funcion = "inet_pton";
	int respuesta = inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr.s_addr)) - 1;

	if (respuesta >= 0) {
		funcion = "socket";
		respuesta = -1;
		socketD = socket(AF_INET, SOCK_STREAM, 0);

		if (socketD != INVALID_SOCKET) {
			funcion = "connect";
			respuesta = connect(socketD, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

			if (respuesta == 0) {
				//std::cout << "Conectado a " << host << ":" << puerto << std::endl;
				conectado = true;
				Conexion conAux(socketD);
				con = conAux;
			}
		}
	}

	if (respuesta < 0) {
		std::string error = "Error llamando a " + funcion + " con código " + std::to_string(getLastError());
		std::cerr << error << std::endl;
	}

}

void Cliente::clrScrn() {
#ifdef _WIN32
	system("cls");
#endif
}

void Cliente::iniciar() {
	leerComando();
}

void Cliente::leerComando() {
	std::string msg;
	std::string resp;
	char opcion = '0';
	while (opcion != '6') {
		opcion = imprimirMenu();
		switch (opcion) {
		case '1':
			if (!conectado)
				conectar();
			loguear();
			break;
		case '2':
			desconectar();
			break;
		case '3':
			enviarMensaje();
			break;
		case '4':
			recibirMensajes();
			break;
		case '5':
			loremIpsum();
			break;
		case '6':
			desconectar();
			break;

		default:
			clrScrn();
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
		if (!logueado) {
			std::cout << "1. Conectar" << std::endl;
		}
		else {
			std::cout << "2. Desconectar" << std::endl;
			std::cout << "3. Enviar" << std::endl;
			std::cout << "4. Recibir" << std::endl;
			std::cout << "5. Lorem Ipsum" << std::endl;
		}
		std::cout << "6. Salir" << std::endl;
		std::cout << "Seleccione una opcion: ";
		std::getline(std::cin, opcion);

		if (opcion.length() == 1) {
			opcionChar = opcion[0];
			correcto = true;
		}
		else {
			clrScrn();
			std::cout << "Opcion Incorrecta" << std::endl;
		}
	}
	return opcionChar;
}

void Cliente::loguear() {
	if (!logueado) {
		while (conectado && !logueado) {
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
					clrScrn();
					std::cout << "Conexion Exitosa." << std::endl;
					parseoUsuario(resp.substr(resp.find('-') + 1, resp.length()));
				}
				else {
					clrScrn();
					std::cout << "Error en la Conexion" << std::endl;
				}
				//std::cout << "El servidor respondio: " << resp << std::endl;
			}
			catch (SocketException e) {
				if (conectado) {
					//Logger::error(std:string(e.what()));
					std::cout << "Conexion con " << host << " cerrada." << std::endl;
					conectado = false;
					logueado = false;
				}
			}
		}
	}
	else {
		clrScrn();
		std::cout << "Ya existe una conexion con el Servidor" << std::endl;
	}
}

void Cliente::parseoUsuario(std::string textoUsuarios) {
	textoUsuarios = textoUsuarios + ",";
	while (textoUsuarios.find(',') != std::string::npos) {
		usuarios.push_back(textoUsuarios.substr(0, textoUsuarios.find(',')));
		textoUsuarios = textoUsuarios.substr(textoUsuarios.find(',') + 1, textoUsuarios.length() - 1);
	}
}

void Cliente::desconectar() {
	if (conectado) {
		conectado = false;
		usuarios.clear();
		shutdown(socketD, 2);
		clrScrn();
		std::cout << "Desconexion Exitosa." << std::endl;
	}
	else {
		clrScrn();
		std::cout << "No existe conexion." << std::endl;
	}
	logueado = false;

}

void Cliente::enviarMensaje() {
	if (conectado && logueado) {
		int index = 1;
		auto it = usuarios.begin();
		while (it < usuarios.end()) {
			std::cout << index << ". " << *it << std::endl;
			it++;
			index++;
		}
		std::cout << index << ". " << "Todos" << std::endl;
		std::cout << std::endl;

		std::cout << "Ingrese destinatario: ";
		std::string destinatario;
		std::getline(std::cin, destinatario);

		index = -1;
		try {
			index = std::stoi(destinatario) - 1;
		}
		catch (std::invalid_argument) {}

		if (index < 0 || (unsigned)index > usuarios.size()) {
			std::cout << "Opcion invalida: " << destinatario << std::endl;
			return;
		}

		std::cout << "Ingrese Mensaje: ";
		std::string texto;
		std::getline(std::cin, texto);
		try {
			CodificadorDeMensajesCliente codificadorDeMensajes(&con);
			codificadorDeMensajes.enviarMensajeFormateado(destinatario, texto);
		}
		catch (SocketException e) {
			if (conectado) {
				//Logger::error(std:string(e.what()));
				std::cout << "Conexion con " << host << " cerrada." << std::endl;
				conectado = false;
			}
		}
	}
	else {
		std::cout << "No hay una Conexion abierta." << std::endl;
	}
}

void Cliente::recibirMensajes() {
	if (conectado && logueado) {
		CodificadorDeMensajesCliente codificadorDeMensajes(&con);
		try {
			codificadorDeMensajes.recibirMensajes();
		}
		catch (SocketException e) {
			if (conectado) {
				//Logger::error(std:string(e.what()));
				std::cout << "Conexion con " << host << " cerrada." << std::endl;
				conectado = false;
			}
		}
	}
	else {
		std::cout << "No hay una Conexion abierta." << std::endl;
	}
}

void Cliente::loremIpsum() {
	if (conectado && logueado) {
		std::string pathFileLoremImpsum = "loremIpsum.txt";
		int frecuenciaDeEnvio;
		int cantidadDeEnvios;
		CodificadorDeMensajesCliente codificadorDeMensajes(&con);
		std::ifstream archivoLoremIpsum(pathFileLoremImpsum);

		std::cout << "Ingrese frecuencia de envio en milisegundos: ";
		std::string entradaUsuario;
		std::getline(std::cin, entradaUsuario);
		frecuenciaDeEnvio = std::stoi(entradaUsuario);					//falta chequeo de tipo de dato int
		std::cout << "Ingrese cantidad de envios maximos: ";
		std::getline(std::cin, entradaUsuario);
		cantidadDeEnvios = std::stoi(entradaUsuario);				//falta chequeo de tipo de dato int

		while (cantidadDeEnvios > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(frecuenciaDeEnvio));
			int longitudMensajeAleatoria, destinatarioAleatorio;
			std::string destinatario;
			std::string texto;
			srand((unsigned)time(NULL));
			destinatarioAleatorio = rand() % usuarios.size();
			destinatario = std::to_string(destinatarioAleatorio + 1);
			srand((unsigned)time(NULL));
			longitudMensajeAleatoria = rand() % RANGO_LONGITUD_MENSAJE_LOREM_IPSUM + 1;
			texto = getMensajeLoremIpsum(archivoLoremIpsum, longitudMensajeAleatoria);
			try {
				codificadorDeMensajes.enviarMensajeFormateado(destinatario, texto);
				cantidadDeEnvios--;
				//clrScrn();
			}
			catch (SocketException e) {
				if (conectado) {
					//Logger::error(std:string(e.what()));
					std::cout << "Conexión con " << host << " cerrada." << std::endl;
					conectado = false;
				}
			}
		}
	}
}

std::string Cliente::getMensajeLoremIpsum(std::ifstream &archivo, int longitudMensaje) {
	char *nuevoMensaje = new char[longitudMensaje];
	std::string nuevoMensajeString;
	if (archivo.eof()) {
		archivo.clear();
		archivo.seekg(0, std::ios::beg);
	}
	archivo.read(nuevoMensaje, longitudMensaje - 1);
	nuevoMensajeString.assign(nuevoMensaje);
	delete nuevoMensaje;
	nuevoMensaje = NULL;
	return nuevoMensajeString;
}
