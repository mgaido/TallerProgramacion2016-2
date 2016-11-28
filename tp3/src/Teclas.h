/*
 * Teclas.h
 *
 *  Created on: 26 sept. 2016
 *      Author: Rod
 */

#ifndef TECLAS_H_
#define TECLAS_H_

const char ARRIBA = 0;
const char IZQ = 1;
const char DER = 2;
const char R = 4;
const char SPACE = 5;

class Teclas {
public:
	Teclas();

	bool arriba();
	bool izq();
	bool der();
	bool recargar();
	bool disparar();

	int getEstado();
	void setEstado(int estado);

	bool evento(char tecla, bool presionada);

private:
	int estado;
};

#endif /* TECLAS_H_ */
