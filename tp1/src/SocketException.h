/*
 * SocketException.h
 *
 *  Created on: Sep 5, 2016
 *      Author: rodrigo
 */

#ifndef SOCKETEXCEPTION_H_
#define SOCKETEXCEPTION_H_

#include "stdafx.h"
#include <exception>

class SocketException: public std::exception {
public:
	SocketException(std::string msg);
	virtual const char* what() const throw();
private:
	std::string msg;
};

#endif /* SOCKETEXCEPTION_H_ */
