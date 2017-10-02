#include "socket.h"
#include <iostream>
using namespace std;

socket::socket(){

}

socket::~socket() {
	
}

inline void socket::say()
{
	cout << "socket::hello" << endl;
}
