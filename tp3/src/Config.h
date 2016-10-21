#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "Bytes.h"
#include "Objeto.h"
#include "Utils.h"

struct ConfigCapa {
	std::array<char, 512> imagen;
	int zIndex;
};

struct ConfigSprite {
	Estado estado;
	std::array<char, 512> imagen;
	int zIndex;
	int frames;
	int tiempo;
};

class Config : public Serializable {
public:
	virtual void toBytes(Bytes &bytes);
	virtual void fromBytes(Bytes &bytes);

	void parsearXML(std::string archivo);

	Punto getTamanioVentana();
	Punto getTamanioJugador();
	double getVelocidadX();
	double getVelocidadY();
	double getGravedad();
	unsigned int getCantidadMinimaJugadores();
	unsigned int getCantidadMaximaJugadores();
	unsigned int getFrameRate();
	unsigned int getNivelPiso();
	unsigned int getLongitud();

	std::vector<ConfigCapa>& getConfigCapas();
	std::vector<ConfigSprite>& getConfigSprites();

	void defaultConfig();

	std::string toString();

private:
	Punto tamanioVentana;
	Punto tamanioJugador;
	unsigned int cantidadMinimaJugadores;
	unsigned int cantidadMaximaJugadores;
	unsigned int longitud;
	unsigned int nivelPiso;
	unsigned int framerate;
	double velocX;
	double velocY;
	double gravedad;
	std::vector<ConfigCapa> configCapas;
	std::vector<ConfigSprite> configSprites;
};

class XmlException : public std::exception {
public:
	XmlException(){};
};

class Nodo {
public:
	Nodo();
	Nodo(rapidxml::xml_node<>* ptr);
	Nodo hijo(std::string nombre);
	std::string valor();
	bool siguiente();
private:
	rapidxml::xml_node<>* ptr;
};
 
#endif // CONFIGURACION_H
