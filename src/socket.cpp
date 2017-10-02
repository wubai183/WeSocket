#include "socket.h"
#include <iostream>
using namespace std;

#ifdef WIN32
#pragma comment(lib, "ws2_32")
#endif // WIN32

namespace {
	class WinSocketComponent
	{
		static bool s_bSocketInited;
	public:
		WinSocketComponent()
		{
			if (s_bSocketInited)
				return;

			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				s_bSocketInited = false;
			else
				s_bSocketInited = true;
		}
		~WinSocketComponent()
		{
			if (s_bSocketInited)
			{
				WSACleanup();
				s_bSocketInited = false;
			}
		}

		static bool valid()
		{
			return s_bSocketInited;
		}
	};

	bool WinSocketComponent::s_bSocketInited = false;
	static WinSocketComponent sWinSocketCompont;
};

We::Socket::Socket()
	: _descriptor(SOCKET_ERROR)
{

}

We::Socket::~Socket()
{

}

void We::Socket::open(Protocol protocol)
{
	assert(_descriptor == SOCKET_ERROR);

	_descriptor = ::socket(AF_INET, protocol, 0);

	CHECK_SOCKET_VALID(_descriptor);;
}

void We::Socket::close()
{
	if (isOpened())
	{
		::closesocket(_descriptor);
		_descriptor = SOCKET_ERROR;
	}
}

void We::Socket::bind(const sockaddr_in & addr)
{
	CHECK_SOCKET_VALID(_descriptor);

	const sockaddr* sockAddr = reinterpret_cast<const sockaddr*>(&addr);
	int error = ::bind(_descriptor, sockAddr, sizeof(addr));
	assert(error != -1);
}

void We::Socket::connect(const sockaddr_in & addr)
{
	CHECK_SOCKET_VALID(_descriptor);

	const sockaddr* sockAddr = reinterpret_cast<const sockaddr*>(&addr);
	int error = ::connect(_descriptor, sockAddr, sizeof(addr));
	assert(error != -1);
}

int We::Socket::send(const char * data, size_t size, int flags)
{
	CHECK_SOCKET_VALID(_descriptor);

	int bytes = ::send(_descriptor, data, size, flags);
	assert(bytes != -1);

	return bytes;
}

int We::Socket::sendto(const sockaddr_in & addr, const char * data, size_t size, int flags)
{
	CHECK_SOCKET_VALID(_descriptor);

	const sockaddr* to = reinterpret_cast<const sockaddr*>(&addr);
	int bytes = ::sendto(_descriptor, data, size, flags, to, sizeof(addr));
	assert(bytes != -1);

	return bytes;
}

int We::Socket::recv(char * data, size_t size, int flags)
{
	CHECK_SOCKET_VALID(_descriptor);

	int bytes = ::recv(_descriptor, data, size, flags);
	assert(bytes != -1);

	return bytes;
}

int We::Socket::recvfrom(sockaddr_in & addr, char * data, int size, int flags)
{
	CHECK_SOCKET_VALID(_descriptor);

	int fromLen = sizeof(addr);
	sockaddr* from = reinterpret_cast<sockaddr*>(&addr);

	int bytes = ::recvfrom(_descriptor, data, size, flags, from, &fromLen);
	assert(bytes != -1);

	return bytes;
}

void We::Socket::listen(int queuelen)
{
	CHECK_SOCKET_VALID(_descriptor);

	int error = ::listen(_descriptor, queuelen);
	assert(error != -1);
}

We::Socket::Descriptor We::Socket::accept(sockaddr_in & addr)
{
	CHECK_SOCKET_VALID(_descriptor);

	int addrLen = sizeof(addr);
	sockaddr* sockAddr = reinterpret_cast<sockaddr*>(&addr);

	Descriptor sfd = ::accept(_descriptor, sockAddr, &addrLen);
	//assert(sfd != -1);

	return sfd;
}

void We::Socket::shutdown(int how)
{
	CHECK_SOCKET_VALID(_descriptor);

	int error = ::shutdown(_descriptor, how);
	assert(error != -1);
}

bool We::Socket::wait(bool & read, bool & write, bool & exception, int seconds, int useconds)
{
	CHECK_SOCKET_VALID(_descriptor);

	struct timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = useconds;

	fd_set* readfds = 0;
	fd_set* writefds = 0;
	fd_set* exceptfds = 0;

	auto new_fd_set = [](Descriptor socket)->fd_set* {
		fd_set* fds = new fd_set();

		FD_ZERO(fds);
		FD_SET(socket, fds);
		return fds;
	};

	if (read) readfds = new_fd_set(_descriptor);
	if (write) writefds = new_fd_set(_descriptor);
	if (exception) exceptfds = new_fd_set(_descriptor);

	int ret = ::select(_descriptor + 1, readfds, writefds, exceptfds, &timeout);

	int errorid = WSAGetLastError();

	if (read)
		read = FD_ISSET(_descriptor, readfds) ? true : false;
	if (write)
		write = FD_ISSET(_descriptor, writefds) ? true : false;
	if (exception)
		exception = FD_ISSET(_descriptor, exceptfds) ? true : false;

	delete readfds;
	delete writefds;
	delete exceptfds;

	assert(ret >= 0);

	return (ret != 0);
}
