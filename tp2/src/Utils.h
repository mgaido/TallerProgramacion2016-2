#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <vector>

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

#endif
