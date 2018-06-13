// UtilNetDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>
#include <thread>
#include <iostream>
using namespace std;

#include <UtilNet\inc\UtilNet.h>
#include <UtilNet\inc\NetCmd.h>
#include <UtilNet\inc\NetSelect.h>

#pragma comment(lib, "UtilNet/lib/UtilNet.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	std::cin.ignore();
	if (false)
	{// tcp
		UTIL_NET::CNetServer cServer;
		cServer.RegisterClientCallBackEx([](SOCKET skt){
			std::cout << "新的客户端:" << skt << std::endl;
			new std::thread([](SOCKET skt){
				UTIL_NET::CNetClient clt;
				clt.Init();
				char szData[1024];
				int len = 0;
				clt.Attach(skt);
				clt.RegisterDataCallBack([&clt](UTIL_NET::CNetCmd* pData, PUTIL_NET_NODE_INFO form){
					char szStr[128];
					memcpy(szStr, pData->GetDataPtr(), pData->GetDataLen());
					szStr[pData->GetDataLen()] = '\0';
					std::cout << "服务器接收到数据: 命令:" << pData->GetMajorCmd() << "\t" << szStr << std::endl << std::endl;
					strncat_s(szStr, _TRUNCATE, " ---SERVER ACK..", _TRUNCATE);
					UTIL_NET::CNetCmd cSendData;
					cSendData = *pData;
					cSendData.SetDataLen(strlen(szStr));
					cSendData.PutData((BYTE*)szStr, strlen(szStr));
					clt.Send(&cSendData);
				});
				while (true)
				{
					Sleep(10);
				}
			}, skt);

		});
		cServer.Init();
		cServer.Open(UTIL_NET::TRANS_PROTOCOL_TYPE_TCP, nullptr, 9888);
		UTIL_NET::CNetClient clt;
		clt.Init();
		clt.Open(UTIL_NET::TRANS_PROTOCOL_TYPE_TCP, "127.0.0.1", 9888);
		clt.RegisterDataCallBack([](UTIL_NET::CNetCmd* pData, PUTIL_NET_NODE_INFO form){
			std::cout << "客户端接收到数据: 长度:" << pData->GetDataLen() << std::endl;
			char szStr[128];
			memcpy(szStr, pData->GetDataPtr(), pData->GetDataLen());
			szStr[pData->GetDataLen()] = '\0';
			std::cout << "客户端接收到数据: 命令:" << pData->GetMajorCmd() << "\t" << szStr << std::endl << std::endl;
		});

		if (!clt.IsError()) {
			std::cout << "链接成功." << std::endl;
		}
		else {
			std::cout << "链接失败." << std::endl;
		}

		std::string str;
		UTIL_NET::CNetCmd cNetData;
		while (true)
		{
			str = "";
			std::cout << "输入:" << std::endl;
			std::cin >> str;

			if (str.length()) {
				cNetData.SetMajorCmd(100);
				cNetData.SetFinish(TRUE);
				cNetData.PutData((BYTE*)str.data(), str.length());
				cNetData.SetDataLen(str.length());
				int len = clt.Send(&cNetData, 2000);
				std::cout << "w:" << len << std::endl;
			}
		}
	}

	if (true)
	{//udp
		UTIL_NET::CNetServer cServer;
		cServer.Init();
		cServer.Open(UTIL_NET::TRANS_PROTOCOL_TYPE_UDP, nullptr, 9889);
		cServer.RegisterDataCallBack([&cServer](UTIL_NET::CNetCmd* pData, PUTIL_NET_NODE_INFO form){
			char szStr[128];
			memcpy(szStr, pData->GetDataPtr(), pData->GetDataLen());
			szStr[pData->GetDataLen()] = '\0';
			std::cout << "服务器接收到数据: 命令:" << pData->GetMajorCmd() << "\t" << szStr << std::endl << std::endl;
			strncat_s(szStr, _TRUNCATE, " ---SERVER ACK..", _TRUNCATE);
			UTIL_NET::CNetCmd cSendData;
			cSendData = *pData;
			cSendData.SetDataLen(strlen(szStr));
			cSendData.PutData((BYTE*)szStr, strlen(szStr));
			cServer.SendToUDP(&cSendData, form->stAddr, sizeof(sockaddr));
		});

		UTIL_NET::CNetClient clt;
		clt.Init();
		clt.Open(UTIL_NET::TRANS_PROTOCOL_TYPE_UDP, "127.0.0.1", 9889);
		clt.RegisterDataCallBack([](UTIL_NET::CNetCmd* pData, PUTIL_NET_NODE_INFO form){
			std::cout << "客户端接收到数据: 长度:" << pData->GetDataLen() << std::endl;
			char szStr[128];
			memcpy(szStr, pData->GetDataPtr(), pData->GetDataLen());
			szStr[pData->GetDataLen()] = '\0';
			std::cout << "客户端接收到数据: 命令:" << pData->GetMajorCmd() << "\t" << szStr << std::endl << std::endl;
		});

		if (!clt.IsError()) {
			std::cout << "链接成功." << std::endl;
		}
		else {
			std::cout << "链接失败." << std::endl;
		}

		std::string str;
		UTIL_NET::CNetCmd cNetData;
		while (true)
		{
			str = "";
			std::cout << "输入:" << std::endl;
			std::cin >> str;

			if (str.length()) {
				cNetData.SetMajorCmd(100);
				cNetData.SetFinish(TRUE);
				cNetData.PutData((BYTE*)str.data(), str.length());
				cNetData.SetDataLen(str.length());
#if 1
				int len = clt.Send(&cNetData, 2000);
#else
				sockaddr stAddr;
				StuffSockAddr(UTIL_NET::TRANS_PROTOCOL_TYPE_UDP, "127.0.0.1", 9889, stAddr);
				int len = clt.SendToUDP(&cNetData, stAddr, sizeof(sockaddr), 2000);
#endif
				std::cout << "w:" << len << std::endl;
			}
		}
	}

	std::cout << "...." << std::endl;
	std::cin.ignore();
	return 0;
}

