#define WE_SOCKET_API_EXPORT

#include "socket.h"
#include <iostream>
#include <thread>
using namespace std;

#pragma comment(lib, "ws2_32")

#define PORT 8888

int main(int argc, char const *argv[])
{
	cout << "..............." << endl;
	We::Socket s;
	s.open(We::Socket::UDP);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s.bind(addr);

	while (true)
	{
		bool bRead = true, bWrite = true, bException = true;
		if (s.wait(bRead, bWrite, bException, 6, 0)) {
			if (bRead) {
				char recvBuf[256] = { 0 };

				sockaddr_in from;
				int bytes = s.recvfrom(from, recvBuf, 256, 0);

				if (bytes > 0) {
					std::cout << recvBuf << endl;
				}
			}
			if (bWrite) {
				char sendBuf[256] = "hello,world!";

				sockaddr_in to;
				to.sin_family = AF_INET;
				to.sin_port = htons(PORT);
				inet_pton(AF_INET, "127.0.0.1", (void*)&to.sin_addr);
				int tolen = sizeof(to);

				int nSend = s.sendto(to, sendBuf, strnlen(sendBuf, 256), 0);
				assert(nSend > 0);

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			if (bException) {

			}
		}
	}
	
	return 0;
}