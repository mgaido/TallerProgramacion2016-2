#include "Config.h"

void Config::parsearXML(std::string archivo) {

	rapidxml::file<> archivoConfigXML(archivo.c_str());
	rapidxml::xml_document<> root;
	root.parse<0>(archivoConfigXML.data());

	Nodo doc = Nodo(&root);
	std::string valor;

	Nodo nodo = doc.hijo("ventana");
	valor = nodo.hijo("ancho").valor();
	tamanioVentana.x = std::stoi(valor);
	valor = nodo.hijo("alto").valor();
	tamanioVentana.y = std::stoi(valor);

	valor = doc.hijo("longitud").valor();
	longitud = std::stoi(valor);

	valor = doc.hijo("piso").valor();
	nivelPiso = std::stoi(valor);

	valor = doc.hijo("maximo_numero_jugadores").valor();
	cantidadMaximaJugadores = std::stoi(valor);

	nodo = doc.hijo("capas").hijo("capa");

	do {
		ConfigCapa capa;

		valor = nodo.hijo("imagen").valor();
		valor.copy(capa.imagen.data(), capa.imagen.size());
		capa.imagen[valor.size()] = '\0';

		valor = nodo.hijo("zindex").valor();
		capa.zIndex = std::stoi(valor);

		this->configCapas.push_back(capa);
	} while (nodo.siguiente());

	nodo = doc.hijo("sprites").hijo("sprite");

	do {
		ConfigSprite sprite;
		valor = nodo.hijo("imagen").valor();
		valor.copy(sprite.imagen.data(), sprite.imagen.size());
		sprite.imagen[valor.size()] = '\0';

		valor = nodo.hijo("zindex").valor();
		sprite.zIndex = std::stoi(valor);

		valor = nodo.hijo("frames").valor();
		sprite.frames = std::stoi(valor);

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

		sprite.estado = estado;
		this->configSprites.push_back(sprite);
	} while (nodo.siguiente());

	debug(toString(), true);
}

void Config::toBytes(Bytes & bytes) {
	bytes.put(tamanioVentana);
	bytes.put(cantidadMaximaJugadores);
	bytes.put(nivelPiso);
	bytes.put(longitud);
	bytes.putAll(configCapas);
	bytes.putAll(configSprites);
}

void Config::fromBytes(Bytes & bytes) {
	bytes.get(tamanioVentana);
	bytes.get(cantidadMaximaJugadores);
	bytes.get(nivelPiso);
	bytes.get(longitud);
	bytes.getAll(configCapas);
	bytes.getAll(configSprites);
}

void Config::defaultConfig() {
	this->cantidadMaximaJugadores = 4;
	this->tamanioVentana.x = 800;
	this->tamanioVentana.y = 600;
	this->longitud = this->tamanioVentana.x * 12;
	this->nivelPiso = 400;
}

std::string Config::toString() {
	std::stringstream ss;

	ss << "Ventana: " << tamanioVentana.x << "x" << tamanioVentana.y << "; ";
	ss << "Longitud: " << longitud << "; ";
	ss << "Nivel piso: " << nivelPiso << "; ";
	ss << "Cantidad maxima jugadores: " << cantidadMaximaJugadores << "; ";

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

int Config::getCantidadMaximaJugadores() {
	return this->cantidadMaximaJugadores;
}

Punto Config::getTamanioVentana() {
	return tamanioVentana;
}

int Config::getNivelPiso() {
	return nivelPiso;
}

int Config::getLongitud() {
	return longitud;
}

Nodo::Nodo(){}

Nodo::Nodo(rapidxml::xml_node<>* ptr) {
	this->ptr = ptr;
}

Nodo Nodo::hijo(std::string nombre) {
	auto child = ptr->first_node(nombre.c_str());
	if (child == 0)
		throw XmlException();
	return Nodo(child);
}

std::string Nodo::valor() {
	return ptr->value();
}

bool Nodo::siguiente() {
	ptr = ptr->next_sibling();
	return ptr != 0;
}

