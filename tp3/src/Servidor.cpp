#include "Servidor.h"

double tickDelay;

Servidor::Servidor(int puerto, std::string archivos, bool inmortal, char modo) {
	this->puerto = puerto;
	this->archivos = split(archivos, ',');
	this->inmortal = inmortal;
	this->modo = modo;
	archivo = this->archivos.begin();
	juego = nullptr;
	detenido = false;
	socketD = INVALID_SOCKET;
}

Servidor::~Servidor() {
	equipos.clear();
}

void Servidor::iniciar() {
	int	response = crearSocket();
	if (response < 0) {
		std::cerr << "No se pudo iniciar la conexion" << std::endl;
	} else {
		crearJuego();
		t_aceptarConexiones = std::thread(&Servidor::aceptarConexiones, this);

		avanzarJuego();

		detener();
	}
}

void Servidor::crearJuego() {
	config = Config();
	config.setInmortal(inmortal);
	config.setModoJuego(modo);
	try {
		config.parsearXML(*archivo);
	} catch (...) {
		warn("Ocurrio un problema leyendo " + *archivo, true);
		config.defaultConfig();
	}

	tickDelay = 1000000.0 / config.getFrameRate();
	juego = new Juego(config);
	info("Juego creado");
}

void Servidor::avanzarJuego() {
	while (! detenido) {
		micros t = tiempo();
		Bytes bytes;
		bytes.put(UPD);

		lockJuego.lock();
		bool cambios = juego->getEstadoObjetos(bytes);
		lockJuego.unlock();

		EstadoJuego estado = juego->getEstado();

		if (estado == EstadoJuego::Ganado|| estado == EstadoJuego::Perdido) {
			cambios = true;

			if (estado == EstadoJuego::Ganado) {
				archivo++;
				if (archivo == archivos.end())
					estado = EstadoJuego::JuegoGanado;
			}

			bytes.put(estado);

			int cantEquipos = equipos.size();
			bytes.put(cantEquipos);
			for (int i = 0; i < cantEquipos; i++)
				bytes.putSerializable(equipos[i]);
		} else {
			bytes.put(estado);
			int aux = 0;
			bytes.put(aux);
		}

		if (cambios) {
			auto it = sesiones.begin();
			while (it != sesiones.end()) {
				(*it)->cambioDeEstado(bytes);
				it++;
			}
		}

		if (estado == EstadoJuego::Ganado|| estado == EstadoJuego::Perdido || estado == EstadoJuego::JuegoGanado) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ESPERA_PUNTOS * 1000));

			if (archivo == archivos.end()) {
				info("Saliendo", true);
				detener();
			} else {
				recargar();
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
		response = bind(socketD, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
		if (response == 0) {
			funcion = "listen";
			response = listen(socketD, 10);
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
			sesiones.push_back(new Sesion(newSocketD, ip, this));
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

Jugador* Servidor::nuevaConexion(Sesion* reqSesion, std::string nombre) {

	bool encontrado = false;
	Jugador* jugador = nullptr;

	lockJuego.lock();

	auto it = sesiones.begin();
	while (it != sesiones.end() && !encontrado) {
		Sesion* sesion = *it;
		if (sesion != reqSesion && sesion->getJugador() != nullptr) {
			encontrado = sesion->getNombre() == nombre;
			if (encontrado && !sesion->estaActiva()) {
				jugador = sesion->getJugador();
				sesion->desconectar();
				delete sesion;
				sesiones.erase(it);
			} else
				it++;
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
		if (juego->getCantdadJugadores() < config.getCantidadMaximaJugadores()) {

			Equipo* equipo = nullptr;
			for (auto unEquipo = equipos.begin(); unEquipo != equipos.end(); unEquipo++) {
				if (unEquipo->tieneJugador(nombre)) {
					equipo = &(*unEquipo);
					break;
				}
			}

			if (equipo == nullptr) {
				if (modo == MODO_GRUPAL) {
					if (equipos.size() == 0)
						equipos.push_back(Equipo());
					equipo = &(equipos[0]);
				} else if (modo == MODO_COOP) {
					if (equipos.size() <= 1)
						equipos.push_back(Equipo());

					int index = equipos.size() == 1 ? 0 : (equipos[0].cantidadDeJugadores() > equipos[1].cantidadDeJugadores() ? 1 : 0);
					equipo = &(equipos[index]);
				} else {
					equipos.push_back(Equipo());
					equipo = &(equipos[0]);
				}
				equipo->agregarJugador(nombre);
			}
			jugador = juego->nuevoJugador(nombre, & (equipo->getPuntajes()[nombre]));

			info("Jugador agregado con nombre " + nombre, true);
		} else {
			info("Ya existen demasiados jugadores, no se puede agregar a " + nombre, true);
		}
	}

	lockJuego.unlock();

	return jugador;
}

void Servidor::recargar() {
	info("Recargando escenario", true);
	lockJuego.lock();
	auto it = sesiones.begin();
	while (it != sesiones.end()) {
		(*it)->recargar();
		it++;
	}

	if (juego != nullptr) {
		delete juego;
		juego = nullptr;
	}

	crearJuego();

	lockJuego.unlock();
}

void Servidor::detener() {
	detenido = true;
	if (socketD != INVALID_SOCKET) {
		closesocket(socketD);
		debug("Socket cerrado");

		if (t_aceptarConexiones.joinable() && t_aceptarConexiones.get_id() != std::this_thread::get_id()) {
			debug("Esperando que termine thread aceptarConexiones");
			t_aceptarConexiones.join();
			debug("Thread aceptarConexiones termino");
		}

		auto it = sesiones.begin();
		while (it != sesiones.end()) {
			(*it)->desconectar();
			delete (*it);
			sesiones.erase(it);
		}
		sesiones.clear();

		if (juego != nullptr) {
			delete juego;
			juego = nullptr;
		}
	}
	info("Servidor detenido");
}

Config& Servidor::getConfiguracion() {
	return config;
}

Juego* Servidor::getJuego() {
	return juego;
}
