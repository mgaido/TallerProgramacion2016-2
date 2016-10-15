#include "Handshake.h"

HandshakeResponse::HandshakeResponse() {
	aceptado = false;
}

void HandshakeResponse::toBytes(Bytes & bytes) {
	bytes.put(aceptado);
	//bytes.putSerializable(configuracion);
}

void HandshakeResponse::fromBytes(Bytes & bytes) {
	bytes.get(aceptado);
	//bytes.getSerializable(configuracion);
}

void Configuracion::toBytes(Bytes & bytes) {
	bytes.putAll(sprites);
	bytes.putAll(fondos);
}

void Configuracion::fromBytes(Bytes & bytes) {
	bytes.getAll(sprites);
	bytes.getAll(fondos);
}
