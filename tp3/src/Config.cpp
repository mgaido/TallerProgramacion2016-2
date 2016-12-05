#include "Config.h"

void Config::parsearXML(std::string archivo) {

	if (archivo.size() == 0) {
		defaultConfig();
		return;
	}

	rapidxml::file<> archivoConfigXML(archivo.c_str());
	rapidxml::xml_document<> root;
	root.parse<0>(archivoConfigXML.data());

	Nodo doc = Nodo(&root).hijo("configuracion");
	std::string valor;

	Nodo nodo = doc.hijo("ventana");
	valor = nodo.hijo("ancho").valor();
	tamanioVentana.x = std::stoi(valor);
	valor = nodo.hijo("alto").valor();
	tamanioVentana.y = std::stoi(valor);

	nodo = doc.hijo("jugador");
	valor = nodo.hijo("ancho").valor();
	tamanioJugador.x = std::stoi(valor);
	valor = nodo.hijo("alto").valor();
	tamanioJugador.y = std::stoi(valor);

	nodo = doc.hijo("velocidad");
	velocX = std::stod(nodo.hijo("x").valor())/1000000.0;
	velocY = std::stod(nodo.hijo("y").valor())/1000000.0;
	gravedad = std::stod(nodo.hijo("g").valor())/1000000000000.0;

	valor = doc.hijo("fps").valor();
	framerate = std::stoi(valor);

	valor = doc.hijo("longitud").valor();
	longitud = std::stoi(valor);

	valor = doc.hijo("piso").valor();
	nivelPiso = std::stoi(valor);

	valor = doc.hijo("minimo_numero_jugadores").valor();
	cantidadMinimaJugadores = std::stoi(valor);

	valor = doc.hijo("maximo_numero_jugadores").valor();
	cantidadMaximaJugadores = std::stoi(valor);

	nodo = doc.hijo("capas").hijo("capa");

	do {
		ConfigCapa capa;

		valor = nodo.hijo("imagen").valor();
		setCharArray(valor, capa.imagen);

		valor = nodo.hijo("zindex").valor();
		capa.zIndex = std::stoi(valor);

		this->configCapas.push_back(capa);
	} while (nodo.siguiente());

	nodo = doc.hijo("sprites").hijo("sprite");

	do {
		ConfigSprite sprite;
		valor = nodo.hijo("imagen").valor();
		setCharArray(valor, sprite.imagen);

		valor = nodo.hijo("zindex").valor();
		sprite.zIndex = std::stoi(valor);

		valor = nodo.hijo("frames").valor();
		sprite.frames = std::stoi(valor);

		valor = nodo.hijo("tiempo").valor();
		sprite.tiempo = std::stoi(valor);

		valor = nodo.hijo("estado").valor();
		Estado estado;
		if (valor == "Caminando")
			estado = Estado::Caminando;

		if (valor == "Saltando")
			estado = Estado::Saltando;

		if (valor == "Quieto")
			estado = Estado::Quieto;

		if (valor == "Desconectado")
			estado = Estado::Desconectado;

		if (valor == "ProyectilEnMovimiento")
			estado = Estado::ProyectilEnMovimiento;

		if (valor == "Bonus")
			estado = Estado::Bonus;

		valor = nodo.hijo("elemento").valor();
		Tipo tipo;
		if (valor == "Jugador")
			tipo = Tipo::Jugador;

		if (valor == "Enemigo")
			tipo = Tipo::Enemigo;

		if (valor == "Boss1")
			tipo = Tipo::Boss1;

		if (valor == "Boss2")
			tipo = Tipo::Boss2;

		if (valor == "Boss3")
			tipo = Tipo::Boss3;

		if (valor == "BonusKill")
			tipo = Tipo::BonusKill;

		if (valor == "BonusVida")
			tipo = Tipo::BonusVida;

		if (valor == "BonusArma")
			tipo = Tipo::BonusArma;

		if (valor == "GunH")
			tipo = Tipo::GunH;

		if (valor == "GunS")
			tipo = Tipo::GunS;

		if (valor == "GunR")
			tipo = Tipo::GunR;

		if (valor == "GunC")
			tipo = Tipo::GunC;


		sprite.estado = estado;
		sprite.tipo = tipo;
		this->configSprites.push_back(sprite);

	} while (nodo.siguiente());

	//nodo = doc.hijo("plataformas").hijo("plataforma");

	//do {	//Plataformas
	//	Plataformas plataforma;
	//	valor = nodo.hijo("y").valor();
	//	plataforma.punto.y = std::stoi(valor);
	//
	//	valor = nodo.hijo("x").valor();
	//	plataforma.punto.x = std::stoi(valor);
	//
	//	valor = nodo.hijo("ancho").valor();
	//	plataforma.ancho = std::stoi(valor);
	//
	//	this->plataformas.push_back(plataforma);
	//
	//} while (nodo.siguiente());

	debug(toString());
}

void Config::toBytes(Bytes & bytes) {
	bytes.put(tamanioVentana);
	bytes.put(framerate);

	bytes.put(tamanioJugador);
	bytes.put(velocX);
	bytes.put(velocY);
	bytes.put(gravedad);

	bytes.put(cantidadMinimaJugadores);
	bytes.put(cantidadMaximaJugadores);

	bytes.put(nivelPiso);
	bytes.put(longitud);
	bytes.putAll(configCapas);
	bytes.putAll(configSprites);
	bytes.putAll(plataformas);
}

void Config::fromBytes(Bytes & bytes) {
	bytes.get(tamanioVentana);
	bytes.get(framerate);

	bytes.get(tamanioJugador);
	bytes.get(velocX);
	bytes.get(velocY);
	bytes.get(gravedad);

	bytes.get(cantidadMinimaJugadores);
	bytes.get(cantidadMaximaJugadores);

	bytes.get(nivelPiso);
	bytes.get(longitud);
	bytes.getAll(configCapas);
	bytes.getAll(configSprites);
	bytes.getAll(plataformas);
}

void Config::defaultConfig() {
	info("Cargando escenario default");

	this->cantidadMinimaJugadores = 1;
	this->cantidadMaximaJugadores = 4;
	this->tamanioVentana.x = 1024;
	this->tamanioVentana.y = 600;
	this->tamanioJugador.x = 100;
	this->tamanioJugador.y = 120;
	this->velocX = 0.0003;
	this->velocY = 0.001;
	this->gravedad = 2*this->velocY/1000000;
	this->framerate = 30;
	this->longitud = this->tamanioVentana.y * 10;
	this->nivelPiso = (int) tamanioVentana.y * 0.833;

	ConfigSprite sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	sprite.tipo = Tipo::Plataforma;
	setCharArray("img/plataforma.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 9;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Caminando;
	sprite.tipo = Tipo::Jugador;
	setCharArray("img/Personaje.png", sprite.imagen);
	sprite.frames = 12;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::MirarArriba;
	sprite.tipo = Tipo::Jugador;
	setCharArray("img/PersonajeQuietoArriba.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::MirarAbajo;
	sprite.tipo = Tipo::Jugador;
	setCharArray("img/PersonajeQuietoAbajo.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Saltando;
	sprite.tipo = Tipo::Jugador;
	setCharArray("img/Personaje-Saltando.png", sprite.imagen);
	sprite.frames = 12;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	setCharArray("img/PersonajeQuieto.png", sprite.imagen);
	sprite.tipo = Tipo::Jugador;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Desconectado;
	setCharArray("img/PersonajeDesconectado.png", sprite.imagen);
	sprite.tipo = Tipo::Jugador;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Caminando;
	sprite.tipo = Tipo::Enemigo;
	setCharArray("img/EnemigoCaminando.png", sprite.imagen);
	sprite.frames = 16;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Saltando;
	sprite.tipo = Tipo::Enemigo;
	setCharArray("img/EnemigoSaltando.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	setCharArray("img/EnemigoDisparando.png", sprite.imagen);
	sprite.tipo = Tipo::Enemigo;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Caminando;
	sprite.tipo = Tipo::Boss1;
	setCharArray("img/Enemigo HiDo.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	setCharArray("img/Enemigo HiDo.png", sprite.imagen);
	sprite.tipo = Tipo::Boss1;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Caminando;
	sprite.tipo = Tipo::Boss2;
	setCharArray("img/Enemigo AirbusterRiberts.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	setCharArray("img/Enemigo AirbusterRiberts.png", sprite.imagen);
	sprite.tipo = Tipo::Boss2;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Caminando;
	sprite.tipo = Tipo::Boss3;
	setCharArray("img/Enemigo Tani Oh.png", sprite.imagen);
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Quieto;
	setCharArray("img/Enemigo Tani Oh.png", sprite.imagen);
	sprite.tipo = Tipo::Boss3;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::ProyectilEnMovimiento;
	setCharArray("img/bala.png", sprite.imagen);
	sprite.tipo = Tipo::GunH;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::ProyectilEnMovimiento;
	setCharArray("img/bala3.png", sprite.imagen);
	sprite.tipo = Tipo::GunR;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::ProyectilEnMovimiento;
	setCharArray("img/bala2.png", sprite.imagen);
	sprite.tipo = Tipo::GunS;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::ProyectilEnMovimiento;
	setCharArray("img/bala4.png", sprite.imagen);
	sprite.tipo = Tipo::GunC;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::ProyectilEnMovimiento;
	setCharArray("img/bala5.png", sprite.imagen);
	sprite.tipo = Tipo::BalaEnemigo;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite(); 
	sprite.estado = Estado::Bonus;
	setCharArray("img/BonusEnergia.png", sprite.imagen);
	sprite.tipo = Tipo::BonusVida;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Bonus;
	setCharArray("img/BonusKill.png", sprite.imagen);
	sprite.tipo = Tipo::BonusKill;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	sprite = ConfigSprite();
	sprite.estado = Estado::Bonus;
	setCharArray("img/Armas.png", sprite.imagen);
	sprite.tipo = Tipo::BonusArma;
	sprite.frames = 1;
	sprite.tiempo = 1000;
	sprite.zIndex = 10;
	this->configSprites.push_back(sprite);

	Plataformas	plataforma = Plataformas();
	plataforma.punto.y = 200;
	plataforma.punto.x = 80;
	plataforma.ancho = 300;
	this->plataformas.push_back(plataforma);

	plataforma = Plataformas();
	plataforma.punto.y = 200;
	plataforma.punto.x = 800;
	plataforma.ancho = 300;
	this->plataformas.push_back(plataforma);

	plataforma = Plataformas();
	plataforma.punto.y = 200;
	plataforma.punto.x = 1500;
	plataforma.ancho = 400;
	this->plataformas.push_back(plataforma);

	ConfigCapa capa = ConfigCapa();
	setCharArray("img/cielo.png", capa.imagen);
	capa.zIndex = 1;
	this->configCapas.push_back(capa);

	capa = ConfigCapa();
	setCharArray("img/fondo.png", capa.imagen);
	capa.zIndex = 2;
	this->configCapas.push_back(capa);

	debug(toString());
}

std::string Config::toString() {
	std::stringstream ss;

	ss << "Ventana: " << tamanioVentana.x << "x" << tamanioVentana.y << "; ";
	ss << "FPS: " << framerate << "; ";
	ss << "Jugador: " << tamanioJugador.x << "x" << tamanioJugador.y << "; ";
	ss << "Veloc x: " << velocX << " Veloc y:  " << velocY << " Gravedad: " << gravedad << "; ";

	ss << "Longitud: " << longitud << "; ";
	ss << "Nivel piso: " << nivelPiso << "; ";
	ss << "Cantidad maxima jugadores: " << cantidadMaximaJugadores << "; ";
	ss << "Cantidad minima jugadores: " << cantidadMinimaJugadores << "; ";

	auto capa = configCapas.begin();
	while (capa != configCapas.end()){
		ss << "Capa " << std::string(capa->imagen.data()) << " zindex " << capa->zIndex << "; ";
		capa++;
	}

	auto sprite = configSprites.begin();
	while (sprite != configSprites.end()) {

		std::string estado;
		switch (sprite->estado) {
		case Estado::Caminando:
			estado = "Caminando";
			break;
		case Estado::Saltando:
			estado = "Saltando";
			break;
		case Estado::Quieto:
			estado = "Quiero";
			break;
		case Estado::Desconectado:
			estado = "Desconectado";
			break;
		default:
			estado = "????";
		}

		ss << "Sprite " << estado << " " << std::string(sprite->imagen.data()) << " zindex " << sprite->zIndex << " frames "<< sprite->frames << "; ";

		sprite++;
	}

	return ss.str();
}

std::vector<ConfigCapa>& Config::getConfigCapas() {
	return this->configCapas;
}

std::vector<ConfigSprite>& Config::getConfigSprites() {
	return this->configSprites;
}

std::vector<Plataformas>& Config::getPlataformas(){
	return plataformas;
}

unsigned int Config::getCantidadMaximaJugadores() {
	return this->cantidadMaximaJugadores;
}

unsigned int Config::getCantidadMinimaJugadores() {
	return this->cantidadMinimaJugadores;
}

double Config::getVelocidadX() {
	return velocX;
}

double Config::getVelocidadY() {
	return velocY;
}

double Config::getGravedad() {
	return gravedad;
}

unsigned int Config::getFrameRate() {
	return this->framerate;
}

Punto Config::getTamanioVentana() {
	return tamanioVentana;
}

Punto Config::getTamanioJugador() {
	return tamanioJugador;
}

unsigned int Config::getNivelPiso() {
	return nivelPiso;
}

unsigned int Config::getLongitud() {
	return longitud;
}

Nodo::Nodo(){
	this->ptr = nullptr;
}

Nodo::Nodo(rapidxml::xml_node<>* ptr) {
	this->ptr = ptr;
}

Nodo Nodo::hijo(std::string nombre) {
	auto child = ptr->first_node(nombre.c_str());
	if (child == 0) {
		error("No existe nodo con nombre " + nombre, true);
		throw XmlException();
	}
	return Nodo(child);
}

std::string Nodo::valor() {
	return ptr->value();
}

bool Nodo::siguiente() {
	ptr = ptr->next_sibling();
	return ptr != 0;
}
