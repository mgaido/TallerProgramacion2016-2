#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#ifdef __linux__
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "signal.h"
#include "errno.h"
#elif _WIN32
#include "SDKDDKVer.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#endif

#include "Logger.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>


#ifdef __linux__

using SOCKET = int;

extern int INVALID_SOCKET;
extern int SOCKET_ERROR;

inline int getLastError() {
	return errno;
}


inline int closesocket(SOCKET socketD) {
	shutdown(socketD, 2);
	return close(socketD);
}

#elif _WIN32

inline int getLastError() {
	return WSAGetLastError();
}

#endif


#endif // STDAFX_H

