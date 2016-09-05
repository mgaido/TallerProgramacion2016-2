/*
 * SocketException.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#include "SocketException.h"

SocketException::SocketException(std::string msg) {
	this->msg = msg;
}

const char* SocketException::what() const throw() {
	return msg.c_str();
}

