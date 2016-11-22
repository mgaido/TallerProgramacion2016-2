#include "Proyectil.h"

Proyectil::Proyectil() {

}

void Proyectil::recargar() {
	cantidad = cantidad + cantidadRepuesto;
}

int Proyectil::getPuntos() {
	return puntosOtorgados;
}

bool Proyectil::disparar() {
	bool dispara = false;
	if (cantidad > 0) {
		cantidad = cantidad - 1;
		dispara = true;
	}
	return dispara;
}