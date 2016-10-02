/*
 * ColaBloqueante.h
 *
 *  Created on: 3 sept. 2016
 *      Author: Rod
 */

#ifndef COLABLOQUEANTE_H_
#define COLABLOQUEANTE_H_

#include <deque>
#include <mutex>
#include <condition_variable>

struct ColaCerrada {
};

template<typename T>
class ColaBloqueante {
public:
	ColaBloqueante();
	~ColaBloqueante();

	void cerrar();
	bool cerrada();

	void encolar(T elemento);
	T desencolar();

private:
	bool estaCerrada;
	std::deque<T> cola;
	std::mutex mutex;
	std::condition_variable cond;
};

template<typename T>
inline ColaBloqueante<T>::ColaBloqueante() {
	estaCerrada = false;
}

template<typename T>
inline ColaBloqueante<T>::~ColaBloqueante() {
}

template<typename T>
inline void ColaBloqueante<T>::cerrar() {
	estaCerrada = true;
	cond.notify_all();
}

template<typename T>
inline bool ColaBloqueante<T>::cerrada() {
	return estaCerrada && cola.empty();
}

template<typename T>
inline void ColaBloqueante<T>::encolar(T elemento) {
	mutex.lock();
	cola.push_front(elemento);
	mutex.unlock();
	cond.notify_one();
}

template<typename T>
inline T ColaBloqueante<T>::desencolar() {
	std::unique_lock<std::mutex> lock(mutex);
	this->cond.wait(lock, [=] { return estaCerrada || ! cola.empty(); });
	if (! cola.empty()) {
		T elemento = cola.back();
		cola.pop_back();
		lock.unlock();
		return elemento;
	} else
		throw ColaCerrada();
}

#endif /* COLABLOQUEANTE_H_ */
