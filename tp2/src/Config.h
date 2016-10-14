#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "Utils.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

typedef struct ConfigCapa {
	std::string idCapa;
	int zIndexCapa;
};
typedef struct ConfigSprite {
	std::string idSprite;
	int zIndexSprite;
	int anchoSprite;
	int altoSprite;
};

class Config {
public:
	void parsearXML(std::string archivo);
	int getCantidadMaximaJugadores();
	std::vector<ConfigCapa> getConfigCapas();
	std::vector<ConfigSprite> getConfigSprites();
	void defaultConfig();

private:
	int cantidadMaximaJugadores;
	std::vector<ConfigCapa> configCapas;
	std::vector<ConfigSprite> configSprites;
};
 
#endif // CONFIGURACION_H
