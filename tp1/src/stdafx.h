#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#ifdef __linux__
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/poll.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "errno.h"
#elif _WIN32
#include "SDKDDKVer.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

#ifdef __linux__

using SOCKET = int;

extern int INVALID_SOCKET;
extern int SOCKET_ERROR;

inline int getLastError() {
	return errno;
}

#elif _WIN32

inline int getLastError() {
	return WSAGetLastError();
}

inline int poll(pollfd* fd, int nfd, long timeout) {
	return WSAPoll(fd, nfd, timeout);
}

#endif

#endif // STDAFX_H

