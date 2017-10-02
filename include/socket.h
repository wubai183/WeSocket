#pragma once

#ifdef WE_SOCKET_API_EXPORT
#define WE_SOCKET_APT __declspec(dllimport)
#else
#define WE_SOCKET_APT __declspec(dllexport)
#endif // WE_SOCKET_API_EXPORT


class WE_SOCKET_APT socket
{
public:
	socket();
	~socket();

	void say();
	
};