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
		this->idCapas.push_back(child->first_node()->value());
		this->zIndexCapas.push_back(std::stoi(child->first_node()->next_sibling()->value()));
		child = child->next_sibling();
	}

	node = node->next_sibling();
	child = node->first_node();

	while (child)
	{
		this->idSprites.push_back(child->first_node()->value());
		this->zIndexSprites.push_back(std::stoi(child->first_node()->next_sibling()->value()));
		this->anchoSprites.push_back(std::stoi(child->first_node()->next_sibling()->next_sibling()->value()));
		this->altoSprites.push_back(std::stoi(child->first_node()->next_sibling()->next_sibling()->next_sibling()->value()));
		child = child->next_sibling();
	}
	/*
	std::cout << "Cantidad: " << cantidadMaximaJugadores << std::endl;
	std::cout << "IdMapa: " << idCapas.at(0) << std::endl;
	std::cout << "zIndexMapa: " << zIndexCapas.at(0) << std::endl;
	std::cout << "IdSprite: " << idSprites.at(0) << std::endl;
	std::cout << "zSprite: " << zIndexSprites.at(0) << std::endl;
	std::cout << "anchoSprite: " << anchoSprites.at(0) << std::endl;
	std::cout << "altoSprite: " << altoSprites.at(0) << std::endl;*/
}

std::vector<std::string> Config::getIdCapas()
{
	return this->idCapas;
}

std::vector<std::string> Config::getIdSprites()
{
	return this->idSprites;
}

std::vector<int> Config::getZIndexCapas()
{
	return this->zIndexCapas;
}

std::vector<int> Config::getZIndexSprites()
{
	return this->zIndexSprites;
}

std::vector<int> Config::getAnchoSprites()
{
	return this->anchoSprites;
}

std::vector<int> Config::getAltoSprites()
{
	return this->altoSprites;
}
