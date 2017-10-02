#include <iostream>
#include <vector>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#pragma comment(lib, "ws2_32.lib")

class SocketInitiaztion
{
	static bool s_bSocketInited;
public:
	SocketInitiaztion()
	{
		if (s_bSocketInited)
			return;

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			s_bSocketInited = false;
		else
			s_bSocketInited = true;
	}
	~SocketInitiaztion()
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
bool SocketInitiaztion::s_bSocketInited = false;
SocketInitiaztion SocketInitModule;

#define ServerPortA 11111
#define ServerPortB 22222

int totalCount = 0;

struct UDPNode
{
	SOCKET sock;
	int nPort;
public:
	void Init(int port)
	{
		nPort = port;

		sock = socket(AF_INET, SOCK_DGRAM, 0);
		assert(sock != INVALID_SOCKET);

		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(nPort);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
		assert(ret != SOCKET_ERROR);
	}

	void handle_if(fd_set* rfd)
	{
		if (!FD_ISSET(sock, rfd))
			return;

		char recvBuf[256] = { 0 };
		sockaddr_in cli;
		int nRecLen = sizeof(cli);
		int nRecEcho = recvfrom(sock, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&cli, &nRecLen);
		assert(nRecEcho != INVALID_SOCKET);
		//printf("{%d}:port[%d]:%s\n", int(nPort), int(ntohs(cli.sin_port)), recvBuf);
		std::cout << ".";
		++totalCount;
	}

	void write_if(fd_set* wfd) {
		if (!FD_ISSET(sock, wfd))
			return;

		//char buffer[] = "test....str";
		//sockaddr_in cli;
		//std::cout << "+";
	}
};


int main()
{
	fd_set rfd;
	fd_set wfd;
	timeval timeout;

	std::vector<UDPNode> node;
	node.push_back(UDPNode());

	node[0].Init(ServerPortA);
	//node[1].Init(ServerPortB);

	timeout.tv_sec = 6;
	timeout.tv_usec = 0;

	int nNode = node.size();
	//sizeof(node) / sizeof(node[0]);

	while (true)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);

		for (size_t i = 0; i < nNode; i++)
		{
			FD_SET(node[i].sock, &rfd);
			FD_SET(node[i].sock, &wfd);
		}

		int nRet = select(0, &rfd, NULL, NULL, &timeout);
		assert(nRet != SOCKET_ERROR);
		if (nRet == 0)
		{
			printf("timeout....\n");
			break;
		}
		else
		{
			for (size_t i = 0; i < nNode; i++)
			{
				node[i].handle_if(&rfd);
				node[i].write_if(&wfd);
			}
		}
	}

	std::cout << std::endl << totalCount << std::endl;
	return 0;
}

