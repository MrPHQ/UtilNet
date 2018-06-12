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

#endif