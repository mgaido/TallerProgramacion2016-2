#pragma once

#include "Actualizacion.h"
#include "Bytes.h"
#include "Utils.h"

class HandshakeRequest {
public:
	char nombre[256];
};

class Sprite {
	Estado estado;
	char nombre[512];
	Punto tamanio;
};

class Fondo {
	char nombre[512];
	Punto tamanio;
	int zIndex = 0;
};

class Configuracion : public Serializable {
public:
	virtual void toBytes(Bytes &bytes);
	virtual void fromBytes(Bytes &bytes);

	std::vector<Sprite> sprites;
	std::vector<Fondo> fondos;
};
class HandshakeResponse : public Serializable {
public:
	HandshakeResponse();

	virtual void toBytes(Bytes &bytes);
	virtual void fromBytes(Bytes &bytes);

	bool aceptado;
	std::vector<Actualizacion> estado;
	Configuracion configuracion;
};





