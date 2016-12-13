#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <cmath>

using micros = double;

extern 	int contador;
enum class Direccion { IZQUIERDA, DERECHA };

class Punto {
public:
	int x;
	int y;
	Punto(){
		x=0;
		y=0;
	}
};

inline double distancia(Punto pos1, Punto pos2) {
	double distanciaX = 0;
	double distanciaY = 0;

	distanciaX = pos1.x - pos2.x;
	distanciaY = pos1.y - pos2.y;

	return sqrt(distanciaX*distanciaX + distanciaY*distanciaY);
}

inline std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> parts;

	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (getline(ss, item, delim)) {
		parts.push_back(item);
	}
	return parts;
}

inline micros tiempo() {
	return (micros) (std::chrono::system_clock::now().time_since_epoch() / std::chrono::microseconds(1));
}

template <std::size_t N>
inline void setCharArray(std::string s, std::array<char, N>& t) {
	s.copy(t.data(), t.size());
	int index = std::min<int>(s.size(), t.size()-1);
	t[index] = '\0';
}

struct EnumClassHash {
	template<typename T>
	std::size_t operator()(T t) const {
		return static_cast<std::size_t>(t);
	}
};

template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using HashMap = std::unordered_map<Key, T, HashType<Key>>;

#endif
