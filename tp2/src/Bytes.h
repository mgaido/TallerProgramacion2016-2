/*
 * Bytes.h
 *
 *  Created on: 2 oct. 2016
 *      Author: rodrigo
 */

#ifndef BYTES_H_
#define BYTES_H_

#include <sstream>
#include <vector>

class Bytes {
public:
	template <typename T>
	Bytes& put(T valor) {
	    bytes.sputn(reinterpret_cast<const char*>(&valor), sizeof valor);
		return *this;
	}

	template <typename T>
	Bytes& put(std::vector<T> valor) {
		put(valor.size());
		for (int i = 0; i < valor.size(); i++)
			put(valor[i]);
		return *this;
	}

	template <typename T>
	Bytes& get(T& valor) {
	    bytes.sgetn(reinterpret_cast<char*>(&valor), sizeof(valor));
		return *this;
	}

	template <typename T>
	Bytes& get(std::vector<T>& valor) {
		int size=0;
		get(size);
		for (int i = 0; i < size; i++) {
			T item;
			get(item);
			valor.push_back(item);
		}
		return *this;
	}


	void fromCharArray(char* buffer, unsigned int size) {
		bytes.sputn(buffer, size);
	}

	const char* toCharArray() {
		return str.c_str();
	}

	unsigned int size() {
		str = bytes.str();
		return str.size();
	}
private:
	std::stringbuf bytes;
	std::string str;
};

#endif /* BYTES_H_ */
