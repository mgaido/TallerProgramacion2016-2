/*
 * TestColaBloqueante.h
 *
 *  Created on: 3 sept. 2016
 *      Author: Rod
 */

#ifndef TESTCOLABLOQUEANTE_H_
#define TESTCOLABLOQUEANTE_H_

#include "../src/ColaBloqueante.h"
#include <thread>
#include <future>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

class TestColaBloqueante {
public:
	TestColaBloqueante();
	virtual ~TestColaBloqueante();
private:
	ColaBloqueante<long> cb;
	std::mutex mutexConsola;

	void producir(int ciclos);
	long consumir(std::string n);
};

#endif /* TESTCOLABLOQUEANTE_H_ */
