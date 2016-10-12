#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "Utils.h"
#include "../../rapidxml-1.13/rapidxml.hpp"
#include "../../rapidxml-1.13/rapidxml_utils.hpp"

class Config {
public:
	void parsearXML(std::string archivo);
	int getCantidadMaximaJugadores();
	std::vector<std::string> getIdCapas();
	std::vector<std::string> getIdSprites();
	std::vector<int> getZIndexCapas();
	std::vector<int> getZIndexSprites();
	std::vector<int> getAnchoSprites();
	std::vector<int> getAltoSprites();

private:
	int cantidadMaximaJugadores;
	std::vector<std::string> idCapas;
	std::vector<int> zIndexCapas;
	std::vector<std::string> idSprites;
	std::vector<int> zIndexSprites;
	std::vector<int> anchoSprites;
	std::vector<int> altoSprites;
};

#endif // CONFIGURACION_H