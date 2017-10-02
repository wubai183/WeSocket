#define WE_SOCKET_API_EXPORT

#include "socket.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	cout << "hello" << endl;
	socket s;
	s.say();
	return 0;
}