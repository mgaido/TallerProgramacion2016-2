#ifndef UTILS_H
#define UTILS_H

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

#endif
