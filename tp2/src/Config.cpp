#include "Config.h"

void Config::parsearXML(std::string archivo) {

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
