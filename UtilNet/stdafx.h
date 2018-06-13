// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here

#include <utils\utils.h>
#include <utils\Singleton.h>
#include <utils\Thread.h>
#include <utils\ShareMemory.h>
#include <utils\file.h>
#include <utils\LogFile.h>
#include <utils\Time.h>
#include <utils\net.h>
#include <utils\buff.h>
using namespace UTILS::API;

#include "inc/UtilNetDef.h"
#include "internal.h"
#include "inc\UtilNet.h"
#include "inc\NetCmd.h"
#include "inc\NetSelect.h"


#define MSG_INFO(...)	UTILS::API::DEBUG_INFO("UtilNet",__VA_ARGS__)