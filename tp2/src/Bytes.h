/*
 * Bytes.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef BYTES_H_
#define BYTES_H_

#include <memory>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>

#include "Logger.h"

class Bytes {
public:

	Bytes() {
		cursor = 0;
	}

	template <typename T>
	Bytes& put(T valor) {
		const char* var = reinterpret_cast<const char*>(&valor);
		bytes.insert(bytes.end(), var, var + sizeof valor);
		return *this;
	}

	template <typename T>
	Bytes& putAll(std::vector<T> valor) {
		put(valor.size());
		for (unsigned int i = 0; i < valor.size(); i++)
			put(valor[i]);
		return *this;
	}

	template <typename T>
	Bytes& get(T& valor) {
		void* ptr = &bytes[cursor];
		memcpy(&valor, ptr, sizeof valor);
		cursor += sizeof valor;
		return *this;
	}

	template <typename T>
	Bytes& getAll(std::vector<T>& valor) {
		size_t size;
		get(size);
		for (unsigned int i = 0; i < size; i++) {
			T item;
			get(item);
			valor.push_back(item);
		}
		return *this;
	}


	unsigned int size() {
		return bytes.size();
	}

	std::vector<char>& toVector() {
		return bytes;
	}

	void fromVector(std::vector<char> bytes){
		this->bytes = bytes;
		cursor = 0;
	}

private:
	std::vector<char> bytes;
	unsigned int cursor;
};

#endif /* BYTES_H_ */
