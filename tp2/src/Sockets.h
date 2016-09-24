#ifndef SOCKETS_H_
#define SOCKETS_H_

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
#include "winsock2.h"
#include "ws2tcpip.h"
#include "mstcpip.h"
#endif

#include <exception>

class SocketException: public std::exception {
public:
	SocketException(){};
};


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

inline void initSockets(){
	#ifdef __linux__
	signal(SIGPIPE, SIG_IGN);
	#elif _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	#endif
}

inline int setTcpNoDelay(SOCKET socket){
	int activar = 1;
	return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&activar, sizeof(activar));
}

inline int setTcpKeepAlive(SOCKET socket){
	int interval = 1;
	int timeout = 1;

	#ifdef __linux__
	int activar = 1;
	int keepidle=interval;

	int rc = setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&activar, sizeof(activar));

	if (rc == 0)
		rc = setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&keepidle, sizeof(keepidle));

	if (rc == 0)
		rc = setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, (char*)&timeout, sizeof(timeout));

	if (rc == 0)
		rc = setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&interval, sizeof(interval));
	#elif _WIN32

	struct tcp_keepalive config;
	config.onoff = true;
	config.keepaliveinterval = interval * 1000;
	config.keepalivetime = timeout * interval * 1000;
	DWORD dwBytesRet = 0;

	int rc = WSAIoctl(socket, SIO_KEEPALIVE_VALS, &config, sizeof(config), NULL, 0, &dwBytesRet, NULL, NULL);

	#endif

	return rc;
}

inline bool hasError(SOCKET socket) {
	int error = 0;
	#ifdef __linux__
	unsigned
	#endif
	int size = sizeof(error);
	int rc = getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*) &error, &size);
	return rc != 0 || error != 0;
}

#endif // SOCKETS_H

