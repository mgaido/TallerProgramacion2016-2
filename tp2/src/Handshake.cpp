#include "Handshake.h"

HandshakeResponse::HandshakeResponse() {
	aceptado = false;
}

void HandshakeResponse::toBytes(Bytes & bytes) {
	bytes.put(aceptado);
	//bytes.putSerializable(configuracion);
	bytes.putAll(estado);
}

void HandshakeResponse::fromBytes(Bytes & bytes) {
	bytes.get(aceptado);
	//bytes.getSerializable(configuracion);
	bytes.getAll(estado);
}

void Configuracion::toBytes(Bytes & bytes) {
	bytes.putAll(sprites);
	bytes.putAll(fondos);
}

void Configuracion::fromBytes(Bytes & bytes) {
	bytes.getAll(sprites);
	bytes.getAll(fondos);
}
