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

	debug("Ventana " + std::to_string(tamanioVentana.x) + "x" + std::to_string(tamanioVentana.y));

	valor = doc.hijo("longitud").valor();
	longitud = std::stoi(valor);

	debug("Longitud " + std::to_string(longitud));

	valor = doc.hijo("piso").valor();
	nivelPiso = std::stoi(valor);

	debug("Nivel piso " + std::to_string(nivelPiso));

	valor = doc.hijo("maximo_numero_jugadores").valor();
	cantidadMaximaJugadores = std::stoi(valor);

	debug("Cantidad maxima jugadores " + std::to_string(cantidadMaximaJugadores));

	nodo = doc.hijo("capas").hijo("capa");

	do {
		ConfigCapa capa;

		valor = nodo.hijo("imagen").valor();
		valor.copy(capa.imagen.data(), capa.imagen.size());
		capa.imagen[valor.size()] = '\0';

		valor = nodo.hijo("zindex").valor();
		capa.zIndex = std::stoi(valor);

		debug("Capa imagen " + std::string(capa.imagen.data()) + " zindex " +  std::to_string(capa.zIndex));

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

		debug("Sprite imagen " + valor + " " + std::string(sprite.imagen.data()) + " zindex "
				+ std::to_string(sprite.zIndex) + " frames " + std::to_string(sprite.frames));

		this->configSprites.push_back(sprite);
	} while (nodo.siguiente());
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

std::vector<ConfigCapa> Config::getConfigCapas() {
	return this->configCapas;
}

std::vector<ConfigSprite> Config::getConfigSprites() {
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

