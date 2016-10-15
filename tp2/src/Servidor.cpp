#include "Servidor.h"

int tickrate = 60;
double tickDelay = 1000000.0/tickrate;

Servidor::Servidor(int puerto, std::string archivo) {
	this->puerto = puerto;
	this->archivo = archivo;
	config = nullptr;
	juego = nullptr;
	detenido = false;
	socketD = INVALID_SOCKET;
}

Servidor::~Servidor() {
}

void Servidor::iniciar() {
	config = new Config();
	try {
		config->parsearXML(archivo);
	} catch (...) {
		config->defaultConfig();
	}

	int		response = crearSocket();
	if (response < 0) {
		std::cerr << "No se pudo iniciar la conexion" << std::endl;
	} else {
		juego = new Juego();
		t_aceptarConexiones = std::thread(&Servidor::aceptarConexiones, this);
		t_procesarPeticiones = std::thread(&Servidor::procesarPeticiones, this);
		t_juego = std::thread(&Servidor::avanzarJuego, this);
	}
}

void Servidor::avanzarJuego() {
	while (! detenido) {
		micros t = tiempo();
		if (juego->estaIniciado()) {
			Bytes bytes;
			bytes.put(UPD);
			if (juego->getEstado(bytes)) {
				auto it = sesiones.begin();
				while (it != sesiones.end()) {
					(*it)->cambioDeEstado(bytes);
					it++;
				}
			}
		}
		t = (micros) (tickDelay - (tiempo() - t));
		if (t > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds((long) t/1000));
	}
}

int Servidor::crearSocket() {
	int response = -1;

	std::string funcion = "socket";
	socketD = socket(AF_INET, SOCK_STREAM, 0);

	if (socketD != INVALID_SOCKET) {
		response = 0;
		setTcpNoDelay(socketD);
		setTcpKeepAlive(socketD);
		setReuseAddress(socketD);

		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(puerto);

		funcion = "bind";
		response = bind(socketD, (struct sockaddr*) &serverAddress,
				sizeof(serverAddress));
		if (response == 0) {
			funcion = "listen";
			response = listen(socketD, 4);				//config->getCantidadMaximaJugadores()
		}
	}

	if (response < 0) {
		socketD = INVALID_SOCKET;
		error("Error llamando a " + funcion + " con codigo " + std::to_string(getLastError()));
	}

	return response;
}

void Servidor::aceptarConexiones() {
	int response = 0;
	struct sockaddr_in clientAddress;
	int clientAddressLength = sizeof(clientAddress);

	while (! detenido) {
		SOCKET newSocketD = accept(socketD, (struct sockaddr*) &clientAddress,
				(socklen_t*) &clientAddressLength);
		
		if (newSocketD != INVALID_SOCKET) {
			char ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
			Peticion peticion {newSocketD, ip};
			peticiones.encolar(peticion);
		} else if (!detenido)
			response = -1;

		if (response < 0)
			detenido = true;
	}

	if (response < 0) {
		error("Error llamando a accept con codigo " + std::to_string(getLastError()));
	} else {
		debug("Finalizando thread aceptarConexiones");
	}
}

void Servidor::procesarPeticiones() {

	while (!detenido) {
		try {
			Peticion peticion = peticiones.desencolar();

			Conexion con;
			con.setSocket(peticion.socketD);

			Bytes bytes = con.recibir();
			HandshakeRequest req;
			bytes.get(req);
			std::string nombre = std::string(req.nombre);
			debug("Cliente '" + std::string(req.nombre) + "' conectado desde " + peticion.ip);

			HandshakeResponse res;

			bool encontrado = false;
			Jugador* jugador = nullptr;

			auto it = sesiones.begin();
			while (it != sesiones.end() && !encontrado) {
				Sesion* sesion = *it;
				encontrado = sesion->getJugador()->getNombre() == nombre;
				if (encontrado && !sesion->estaActiva()) {
					jugador = sesion->getJugador();
					sesiones.erase(it);
					sesion->desconectar();
					delete sesion;
				} else
					it++;
			}

			if (encontrado) {
				if (jugador == nullptr) {
					info("Ya existe jugador activo con nombre " + nombre, true);
				} else {
					info("Existe jugador inactivo con nombre " + nombre, true);
				}
			} else {
				if (sesiones.size() < (unsigned) config->getCantidadMaximaJugadores()) {
					info("Creando jugador con nombre " + nombre, true);
					jugador = juego->nuevoJugador(nombre);
				} else {
					info("Ya existen demasiados jugadores, no se puede agregar a " + nombre, true);
				}
			}

			if (jugador != nullptr) {
				res.aceptado = true;
				sesiones.push_back(new Sesion(peticion.socketD, peticion.ip, jugador, config));
			} else {
				res.aceptado = false;
			}

			bytes = Bytes();
			bytes.putSerializable(res);
			con.enviar(bytes);

		} catch (ColaCerrada&) {
			break;
		} catch (SocketException&) { }
	}

}

void Servidor::detener() {
	if (!detenido) {
		detenido = true;

		if (socketD != INVALID_SOCKET) {

			closesocket(socketD);
			debug("Socket cerrado");

			debug("Esperando que termine thread aceptarConexiones");
			t_aceptarConexiones.join();
			debug("Thread aceptarConexiones termino");

			peticiones.cerrar();
			debug("Esperando que termine thread procesarPeticiones");
			t_procesarPeticiones.join();
			debug("Thread procesarPeticiones termino");

			debug("Esperando que termine thread juego");
			t_juego.join();
			debug("Thread juego termino");

			auto it = sesiones.begin();
			while (it != sesiones.end()) {
				(*it)->desconectar();
				delete (*it);
				sesiones.erase(it);;
			}
			sesiones.clear();
			delete juego;
		}
		info("Servidor detenido");
	}
}

Juego * Servidor::getJuego() {
	return juego;
}

Config *Servidor::getConfig() {
	return config;
}
