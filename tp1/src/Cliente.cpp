#include "Cliente.h"

Cliente::Cliente(std::string cHost, int cPuerto) {
	conectado = false;
	logueado = false;
	socketD = INVALID_SOCKET;
	puerto = cPuerto;
	host = cHost;
}

void Cliente::iniciar() {
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
		std::cout << std::endl;
		if (!conectado || !logueado) {
			std::cout << "1. Conectar" << std::endl;
		} else {
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
		} else {
			std::cout << "Opcion Incorrecta" << std::endl;
		}
	}
	return opcionChar;
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
				conectado = true;
				con = Conexion(socketD);
				info("Conectado a " + host + ":" + std::to_string(puerto));

			}
		}
	}

	if (respuesta < 0) {
		error("No se pudo establecer la conexion con el Servidor", true);
		error("Error " + std::to_string(getLastError()) + " en " + funcion);
	}
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
				std::stringstream ss;
				ss << AUTH << DELIM << usuario << DELIM << clave;
				con.enviar(ss.str());
				std::vector<std::string> resp = split(con.recibir(), DELIM);

				if (resp[0] == SUCCESS) {
					logueado = true;
					usuarios = split(resp[1], ',');
					info("Conexion exitosa con el Servidor: " + host, true);
				} else {
					warn("Error en la conexion con " + host + ": " + resp[1], true);
				}
			} catch (SocketException) {
				if (conectado) {
					error("Conexion con " + host +" cerrada", true);
					conectado = false;
					logueado = false;
				}
			}
		}
	} else {
		warn("Ya existe una conexion con " + host);
	}
}

void Cliente::desconectar() {
	if (conectado) {
		logueado = false;
		conectado = false;

		std::stringstream ss;
		ss << BYE;
		try {
			con.enviar(ss.str());
			con.recibir();
		} catch (SocketException) {}
		closesocket(socketD);
		info("Desconexion exitosa con: " + host, true);
	} else {
		warn("No hay una Conexion abierta");
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
		} catch (std::invalid_argument) {}

		if (index < 0 || (unsigned)index > usuarios.size()) {
			std::cout << "Opcion invalida: " << destinatario << std::endl;
			return;
		}

		std::cout << "Ingrese Mensaje: ";
		std::string texto;
		std::getline(std::cin, texto);

		enviarMensaje(index, texto);

	} else {
		warn("No hay una Conexion abierta");
	}
}

void Cliente::enviarMensaje(int usuario, std::string texto) {
	std::stringstream ss;
	ss << SEND << DELIM << usuario << DELIM << texto;
	try {
		con.enviar(ss.str());

		std::string s = con.recibir();
		std::vector<std::string> resp = split(s, DELIM);
		if (resp[0] == SUCCESS) {
			info("Mensaje enviado a " + usuarios[usuario] + ": " + texto);
			std::cout << "Mensaje enviado." << std::endl;
		} else {
			error("No se pudo enviar el mensaje: " + resp[1], true);
		}
	} catch (SocketException) {
		if (conectado) {
			conectado = false;
			logueado = false;
			error("Error al enviar mensaje", true);
			error("Conexion con " + host + " cerrada", true);
		}
	}
}

void Cliente::recibirMensajes() {
	if (conectado && logueado) {
		std::stringstream ss;
		ss << RETR;
		try {
			con.enviar(ss.str());
			std::vector<std::string> resp = split(con.recibir(), DELIM);
			if (resp[0] == SUCCESS) {
				resp.erase(resp.begin());

				if (resp.size() % 3 != 0) {
					error("Error al recibir mensajes", true);
					error("Cantidad de parámetros incorrecta: " + std::to_string(resp.size()));
				} else {
					std::cout << "Hay " << resp.size() / 3 << " mensajes:" << std::endl;
					info("Se recuperaron " + std::to_string(resp.size() / 3) + " mensajes");

					int cantMensajes = resp.size();
					for (int i = 0; i < cantMensajes; i += 3) {
						std::cout << std::endl << "Destinatario: " << resp[i + 1];
						std::cout << std::endl << "Remitente: " << resp[i];
						std::cout << std::endl << std::endl << resp[i + 2] << std::endl << std::endl;
					}
				}
			} else {
				error("Error al recibir mensajes: " + resp[1], true);
			}
		} catch (SocketException) {
			if (conectado) {
				logueado = false;
				conectado = false;
				error("Error al recibir mensajes", true);
				error("Conexion con " + host + " cerrada", true);
			}
		}
	} else {
		warn("No hay una Conexion abierta");
	}
}

void Cliente::loremIpsum() {
	if (conectado && logueado) {

		int frecuenciaDeEnvio=-1;
		while (frecuenciaDeEnvio < 0) {
			std::cout << "Ingrese frecuencia de envio por segundo: ";
			std::string entradaUsuario;
			std::getline(std::cin, entradaUsuario);
			try {
				frecuenciaDeEnvio = std::stoi(entradaUsuario);
			} catch (std::invalid_argument) {}
		}

		int cantidadDeEnvios=-1;
		while (cantidadDeEnvios < 0) {
			std::cout << "Ingrese cantidad de envios mensajes: ";
			std::string entradaUsuario;
			std::getline(std::cin, entradaUsuario);
			try {
				cantidadDeEnvios = std::stoi(entradaUsuario);
			} catch (std::invalid_argument) {}
		}

		bool good = false;
		std::ifstream archivoLoremIpsum;
		while (! good) {
			std::cout << "Ingrese el nombre del archivo [loremIpsum.txt]: ";
			std::string pathFileLoremImpsum;
			std::getline(std::cin, pathFileLoremImpsum);
			if (pathFileLoremImpsum.empty())
				pathFileLoremImpsum = "loremIpsum.txt";
			archivoLoremIpsum = std::ifstream(pathFileLoremImpsum);
			good = archivoLoremIpsum.good();
		}

		int longitudMensajeAleatoria, destinatarioAleatorio;
		destinatarioAleatorio = rand() % usuarios.size();
		srand((unsigned)time(NULL));
		longitudMensajeAleatoria = rand() % RANGO_LONGITUD_MENSAJE_LOREM_IPSUM + 1;

		auto total = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

		long pausa = 0;
		while (conectado && cantidadDeEnvios > 0) {
 			if (pausa > 0)
				std::this_thread::sleep_for(std::chrono::milliseconds(pausa));

 			auto time = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

			std::string texto = getMensajeLoremIpsum(archivoLoremIpsum, longitudMensajeAleatoria);
			enviarMensaje(destinatarioAleatorio, texto);

			time = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) - time;
			pausa = (unsigned long) (1000 / (double) frecuenciaDeEnvio) - time;

			cantidadDeEnvios--;
		}
		archivoLoremIpsum.close();
		if (conectado && logueado) {
			total = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) - total;
			info("LoremIsum enviado correctamente en " + std::to_string(total) + "ms", true);
		}
	} else {
		warn("No hay una Conexion abierta");
	}
}

std::string Cliente::getMensajeLoremIpsum(std::ifstream &archivo, int longitudMensaje) {
	char *nuevoMensaje = new char[longitudMensaje+1];
	std::string nuevoMensajeString;
	if (archivo.eof()) {
		archivo.clear();
		archivo.seekg(0, std::ios::beg);
	}
	archivo.read(nuevoMensaje, longitudMensaje);
	nuevoMensaje[longitudMensaje] = '\0';
	nuevoMensajeString.assign(nuevoMensaje);
	delete nuevoMensaje;
	nuevoMensaje = NULL;
	return nuevoMensajeString;
}
