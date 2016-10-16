#pragma once

#include "Bytes.h"
#include "Config.h"
#include "Utils.h"

class HandshakeRequest {
public:
	std::string getNombre();
	void setNombre(std::string nombre);

private:
	std::array<char, 100> nombre;
};

class HandshakeResponse : public Serializable {
public:
	HandshakeResponse();

	virtual void toBytes(Bytes &bytes);
	virtual void fromBytes(Bytes &bytes);

	bool isAceptado() const;
	void setAceptado(bool aceptado);

	int getIdJugador() const;
	void setIdJugador(int idJugador);

	Config& getConfiguracion();
	void setConfiguracion(Config& configuracion);


private:
	bool aceptado;
	int idJugador;
	Config configuracion;
};





