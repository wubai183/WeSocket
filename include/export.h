#ifndef __WE_SOCKET_EXPORT_H__
#define __WE_SOCKET_EXPORT_H__

#pragma once

#ifdef WIN32

#ifdef WE_SOCKET_API_EXPORT
#define WE_SOCKET_APT __declspec(dllimport)
#else
#define WE_SOCKET_APT __declspec(dllexport)
#endif // WE_SOCKET_API_EXPORT

#endif

#endif //__WE_SOCKET_EXPORT_H__
