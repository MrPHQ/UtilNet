#ifndef __UTIL_NET__
#define __UTIL_NET__

#ifdef UTIL_NET_EXPORTS
#define UTIL_NET_CAPI extern "C" __declspec(dllexport)
#else
#define UTIL_NET_CAPI extern "C" __declspec(dllimport)	
#endif

#ifdef UTIL_NET_EXPORTS
#define UTIL_NET_CPPAPI __declspec(dllexport)
#else
#define UTIL_NET_CPPAPI __declspec(dllimport)	
#endif
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <Winsock2.h>
#include <Ws2TcpIp.h>
#include <utils\utils.h>
#include <utils\Singleton.h>
#include <utils\Thread.h>
#include <utils\Time.h>
#include <utils\net.h>
#include <utils\buff.h>
using namespace UTILS::API;

#include <UtilNet\inc\UtilNetDef.h>

#endif