/*
 * TestColaBloqueante.cpp
 *
 *  Created on: 3 sept. 2016
 *      Author: Rod
 */

#include "TestColaBloqueante.h"

TestColaBloqueante::TestColaBloqueante() {

	auto c1 = std::async(&TestColaBloqueante::consumir, this, "1");
	auto c2 = std::async(&TestColaBloqueante::consumir, this, "2");
	auto c3 = std::async(&TestColaBloqueante::consumir, this, "3");
	auto c4 = std::async(&TestColaBloqueante::consumir, this, "4");

	int ciclos = 20000;
	std::thread p(&TestColaBloqueante::producir, this, ciclos);

	long suma = c1.get() + c2.get() + c3.get() + c4.get();
	p.join();

	std::cout << "Suma consumirores: " << suma << " Debe dar " << (ciclos*(ciclos-1))/2 << std::endl;
}

TestColaBloqueante::~TestColaBloqueante() {
}

void TestColaBloqueante::producir(int ciclos) {
	for (long i=0; i < ciclos; i++) {
		cb.encolar(i);
	}
	cb.cerrar();

	mutexConsola.lock();
	std::cout << "Productor finalizó." << std::endl;
	mutexConsola.unlock();
}

long TestColaBloqueante::consumir(std::string n) {
	long suma = 0;

	while (true) {
		try {
			long v = cb.desencolar();
			suma += v;
			if (v % 2 == 0)
				std::this_thread::sleep_for(std::chrono::microseconds(1));
		} catch (ColaCerrada) {
			break;
		}
	}

	mutexConsola.lock();
	std::cout << "Consumidor " << n << " finalizó. Suma " << suma << std::endl;
	mutexConsola.unlock();

	return suma;
}

/*
int main(int argc, char *argv[]) {
	TestColaBloqueante tcb;
	return 0;
}
*/
