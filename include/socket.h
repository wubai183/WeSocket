#ifndef __WSOCKET_SOCKET_H__
#define __WSOCKET_SOCKET_H__

#pragma once
#include "export.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <assert.h>

#define CHECK_SOCKET_VALID(FD) \
	assert((FD) != SOCKET_ERROR);\
	assert((FD) != INVALID_SOCKET)

namespace We {

	struct SockAddrCast
	{
		const sockaddr* _sockAddr;
		size_t _size;

		SockAddrCast(const sockaddr_in& addr)
			: _sockAddr(reinterpret_cast<const sockaddr*>(&addr))
			, _size(sizeof(addr))
		{

		}

		const sockaddr* addr() const { return _sockAddr; }
		size_t size() const { return _size; }
	};

	class WE_SOCKET_APT Socket
	{
	public:
		enum Protocol
		{
			TCP = SOCK_STREAM,
			UDP = SOCK_DGRAM
		};

		typedef int Descriptor;

		Socket();

		~Socket();

		void open(Protocol protocol);

		operator Descriptor() { return _descriptor; }
		bool isOpened() { return _descriptor > 0; }

		void close();

		void bind(const sockaddr_in& addr);

		void connect(const sockaddr_in& addr);

		int send(const char* data, size_t size, int flags);

		int sendto(const sockaddr_in& addr, const char* data, size_t size, int flags);

		int recv(char* data, size_t size, int flags);

		int recvfrom(sockaddr_in& addr, char* data, int size, int flags);

		void listen(int queuelen = 5);

		Descriptor accept(sockaddr_in& addr);

		//getsockname();
		//getpeername();

		void shutdown(int how);

		bool wait(bool& read, bool& write, bool& exception, int seconds, int useconds);

	private:
		Descriptor _descriptor;
	};
};

#endif // !__WSOCKET_SOCKET_H__
