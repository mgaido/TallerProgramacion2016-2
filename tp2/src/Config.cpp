#include "Config.h"

void Config::parsearXML(std::string archivo) {//falta chequeo de archivo mal formado
		rapidxml::file<> archivoConfigXML(archivo.c_str());
		rapidxml::xml_document<> configuracion;
		configuracion.parse<0>(archivoConfigXML.data());

		std::string valorDelNodoXML = configuracion.first_node()->first_node()->value();
		this->cantidadMaximaJugadores = std::stoi(valorDelNodoXML);

		rapidxml::xml_node<>* node = configuracion.first_node("configuracion")->first_node()->next_sibling();
		rapidxml::xml_node<> *child = node->first_node();

		while (child)
		{
			ConfigCapa nuevaConfigDeCapa;
			nuevaConfigDeCapa.idCapa = child->first_node()->value();
			nuevaConfigDeCapa.zIndexCapa = std::stoi(child->first_node()->next_sibling()->value());
			this->configCapas.push_back(nuevaConfigDeCapa);
			child = child->next_sibling();
		}

		node = node->next_sibling();
		child = node->first_node();

		while (child)
		{
			ConfigSprite nuevaCondifDeSprite;
			nuevaCondifDeSprite.idSprite = child->first_node()->value();
			nuevaCondifDeSprite.zIndexSprite = std::stoi(child->first_node()->next_sibling()->value());
			nuevaCondifDeSprite.anchoSprite = std::stoi(child->first_node()->next_sibling()->next_sibling()->value());
			nuevaCondifDeSprite.altoSprite = std::stoi(child->first_node()->next_sibling()->next_sibling()->next_sibling()->value());
			this->configSprites.push_back(nuevaCondifDeSprite);
			child = child->next_sibling();
		}
		/*
		std::cout << "Cantidad: " << cantidadMaximaJugadores << std::endl;
		std::cout << "IdMapa: " << configCapas.at(0).idCapa << std::endl;
		std::cout << "zIndexMapa: " << configCapas.at(0).zIndexCapa << std::endl;
		std::cout << "IdSprite: " << configSprites.at(0).idSprite << std::endl;
		std::cout << "zSprite: " << configSprites.at(0).zIndexSprite << std::endl;
		std::cout << "anchoSprite: " << configSprites.at(0).anchoSprite << std::endl;
		std::cout << "altoSprite: " << configSprites.at(0).altoSprite << std::endl;
		*/
}

void Config::toBytes(Bytes & bytes) {
	bytes.putAll(configCapas);
	bytes.putAll(configSprites);
}

void Config::fromBytes(Bytes & bytes) {
	bytes.getAll(configCapas);
	bytes.getAll(configSprites);
}


void Config::defaultConfig() {
	this->cantidadMaximaJugadores = 4;
}

std::vector<ConfigCapa> Config::getConfigCapas(){
	return this->configCapas;
}

std::vector<ConfigSprite> Config::getConfigSprites(){
	return this->configSprites;
}

int Config::getCantidadMaximaJugadores() {
	return this->cantidadMaximaJugadores;
}