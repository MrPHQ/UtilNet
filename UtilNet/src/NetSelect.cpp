#include "../stdafx.h"
#include "../inc/NetSelect.h"

namespace UTIL_NET
{
	CNetBase::CNetBase()
		:m_bInit(FALSE)
	{
	}

	CNetBase::~CNetBase()
	{
		UnInit();
	}

	int CNetBase::Init(unsigned int uiBufferSize /*= DEFAULT_NET_DATA_BUFFER_LEN*/)
	{
		if (m_bInit){
			return UTIL_NET_ERR_SUCCESS;
		}
		m_buff.Init(uiBufferSize);
		m_tbNet.Init(0, 0, TRUE, TRUE);
		m_tbNet.Start([this](BOOL& bRun, HANDLE hWait, void* context){
			CNetCmd cNetData;
			UTILS::CTimeStampWrapper tsCheckStatus;
			tsCheckStatus.Init(GetTickCount(), 5000);
			while (bRun)
			{
				WaitForSingleObject(hWait, 10);
				if (!bRun){
					break;
				}
				// 检测网络连接状态
				if(tsCheckStatus.Check(GetTickCount())){
					CheckStatus();
				}
				// 监听/数据接收线程.
				CheckThread(bRun, &cNetData);
			}
		});
		m_bInit = TRUE;
		return UTIL_NET_ERR_SUCCESS;
	}

	void CNetBase::UnInit()
	{
		if (!m_bInit){
			return;
		}
		m_bInit = FALSE;
		m_tbNet.Cancel();
		m_tbNet.Stop(500);
		m_tbNet.UnInit();
		m_buff.UnInit();
	}

	void CNetBase::Close()
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		GetObjectIns().Close();
	}

	void CNetBase::RegisterStatusCallBack(std::function<void(BOOL bStatus)> cb)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		m_cbStatusCallback = cb;
	}

	void CNetBase::RegisterDataCallBack(std::function<void(CNetCmd*, PUTIL_NET_NODE_INFO form)> cb)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		m_cbDataCallback = cb;
	}

	int CNetBase::Send(CNetCmd* pDataBuff, unsigned int uiTimeOut /*= 5000*/, bool bLocked /*= false*/)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex(),std::defer_lock);
		if (!bLocked){
			lck.lock();
		}
		if (nullptr == pDataBuff){
			return UTIL_NET_ERR_PAR;
		}
		return GetObjectIns().Write((const char*)pDataBuff->GetBufferPtr(), pDataBuff->GetDataTotalLen(), uiTimeOut);
	}

	int CNetBase::SendEx(CNetCmd* pDataBuff, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut/* = 5000*/)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());

		UTILS::CTimeStampWrapper tsTimeOut;
		CNetCmd* pNetCmdPtr = nullptr;
		int iSendTotolLen = 0, iSendLen = 0, iRet = 0;

		tsTimeOut.Init(GetTickCount(), uiTimeOut);

		if (pDataBuff != nullptr){
			iSendLen = Send(pDataBuff, uiTimeOut, true);
			iSendTotolLen += iSendLen;
			if (IsError()){
				if (err != nullptr){
					*err = UTIL_NET_ERR_FAIL;
				}
				return iSendTotolLen;
			}
			if (tsTimeOut.Check(GetTickCount(), false)){
				if (err != nullptr){
					*err = UTIL_NET_ERR_TIMEOUT;
				}
				return iSendTotolLen;
			}
		}

		if (cb){
			while (true)
			{
				iRet = cb(pNetCmdPtr);
				if ((iRet == 0) && (pNetCmdPtr != nullptr)){
					iSendLen = Send(pNetCmdPtr, uiTimeOut, true);
					iSendTotolLen += iSendLen;

					if (IsError()){
						if (err != nullptr){
							*err = UTIL_NET_ERR_FAIL;
						}
						return iSendTotolLen;
					}
					if (tsTimeOut.Check(GetTickCount(), false)){
						if (err != nullptr){
							*err = UTIL_NET_ERR_TIMEOUT;
						}
						return iSendTotolLen;
					}
				}
				else{
					break;
				}
			}
		}
		return iSendTotolLen;
	}

	int CNetBase::SendToUDP(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, unsigned int uiTimeOut /*= 5000*/, bool bLocked /*= false*/)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex(), std::defer_lock);
		if (!bLocked){
			lck.lock();
		}
		if (nullptr == pDataBuff){
			return UTIL_NET_ERR_PAR;
		}
		return GetObjectIns().WriteToUDP((const char*)pDataBuff->GetBufferPtr(), pDataBuff->GetDataTotalLen(), to, tolen, uiTimeOut);
	}

	int CNetBase::SendToUDPEx(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut /*= 5000*/)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());

		UTILS::CTimeStampWrapper tsTimeOut;
		CNetCmd* pNetCmdPtr = nullptr;
		int iSendTotolLen = 0, iSendLen = 0, iRet = 0;

		tsTimeOut.Init(GetTickCount(), uiTimeOut);
		if (pDataBuff != nullptr){
			iSendLen = SendToUDP(pDataBuff, to, tolen, uiTimeOut, true);
			iSendTotolLen += iSendLen;
			if (IsError()){
				if (err != nullptr){
					*err = UTIL_NET_ERR_FAIL;
				}
				return iSendTotolLen;
			}
			if (tsTimeOut.Check(GetTickCount(), false)){
				if (err != nullptr){
					*err = UTIL_NET_ERR_TIMEOUT;
				}
				return iSendTotolLen;
			}
		}

		if (cb){
			while (true)
			{
				iRet = cb(pNetCmdPtr);
				if ((iRet == 0) && (pNetCmdPtr != nullptr)){
					iSendLen = SendToUDP(pDataBuff, to, tolen, uiTimeOut, true);
					iSendTotolLen += iSendLen;

					if (IsError()){
						if (err != nullptr){
							*err = UTIL_NET_ERR_FAIL;
						}
						return iSendTotolLen;
					}
					if (tsTimeOut.Check(GetTickCount(), false)){
						if (err != nullptr){
							*err = UTIL_NET_ERR_TIMEOUT;
						}
						return iSendTotolLen;
					}
				}
				else{
					break;
				}
			}
		}
		return iSendTotolLen;
	}

	bool CNetBase::CheckStatus()
	{
		if (IsError()){
			if (m_cbStatusCallback){
				m_cbStatusCallback(FALSE);
			}
			return false;
		}
		return true;
	}

	void CNetBase::CheckThread(BOOL& bRun, CNetCmd* pBuffer)
	{

	}

	void CNetBase::RecvData(CNetCmd* pBuffer)
	{
		if (nullptr == pBuffer){
			return;
		}
		if (IsError()){
			return;
		}
		if (m_buff.SpaceEx() <= 0){
			MSG_INFO("ERROR Buffer .LINE:%d", __LINE__);
			return;
		}
		char* pBuffData = nullptr;
		size_t uiNeedReadDataLen = 0, uiReadLen = 0;
		PNET_DATA_HEAD pCommData;
		int iDataLen = 0;
		PUTIL_NET_NODE_INFO pNodeInfo = nullptr;
		UTILS::NET::TRANS_PROTOCOL_TYPE nType = GetObjectIns().GetTransProtocol();
		if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_TCP){
			iDataLen = GetObjectIns().Read((char*)m_buff.GetDataEnd(), m_buff.SpaceEx(), 0, 2000);
		}
		else if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_UDP)
		{
			sockaddr from;
			int len = sizeof(sockaddr);
			iDataLen = GetObjectIns().ReadFromUDP((char*)m_buff.GetDataEnd(), m_buff.SpaceEx(), from, len, 2000);
			if (iDataLen > 0)
			{
				UTIL_NET_NODE_INFO stNodeInfo;
				pNodeInfo = &stNodeInfo;
				ZeroMemory(pNodeInfo, sizeof(UTIL_NET_NODE_INFO));
				pNodeInfo->uiSize = sizeof(UTIL_NET_NODE_INFO);
				memcpy(&pNodeInfo->stAddr, &from, sizeof(sockaddr));
				UTILS::NET::GetSktNameEx(from, pNodeInfo->szIp, sizeof(pNodeInfo->szIp), &pNodeInfo->iPort);
			}
		}

		if (iDataLen <= 0) {
			return;
		}

		m_buff.Write((size_t)iDataLen);

		do
		{
			if (m_buff.Size() < sizeof(NET_DATA_HEAD)) {
				break;
			}
			pCommData = (PNET_DATA_HEAD)(m_buff.GetDataStart());
			if (pCommData->uiSize != sizeof(NET_DATA_HEAD)) {
				m_buff.Clear();
				MSG_INFO("ERROR .Clear Data.LINE:%d", __LINE__);
				break;
			}

			if (pCommData->uiSize + pCommData->uiDataLen > m_buff.Size()) {
				break;
			}
			pBuffData = nullptr;
			uiNeedReadDataLen = pCommData->uiSize + pCommData->uiDataLen;
			uiReadLen = m_buff.ReadEx(pBuffData, uiNeedReadDataLen);
			if ((uiReadLen != uiNeedReadDataLen) || (pBuffData == nullptr)) {
				MSG_INFO("ERROR .Clear Data.LINE:%d", __LINE__);
				m_buff.Clear();
				break;
			}
			pBuffer->Attach((BYTE*)pBuffData, uiReadLen);
			if (!pBuffer->Verity()) {
				MSG_INFO("Net Data Error LINE:%d", __LINE__);
				break;
			}

			if (m_cbDataCallback){
				m_cbDataCallback(pBuffer, pNodeInfo);
			}
		} while (true);
	}

	CNetServer::CNetServer()
		:CNetBase()
	{
	}

	CNetServer::~CNetServer()
	{
	}

	bool CNetServer::Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		UTILS::NET::TRANS_PROTOCOL_TYPE nType = UTILS::NET::TRANS_PROTOCOL_TYPE_NONE;
		switch (nProType)
		{
		case UTIL_NET::TRANS_PROTOCOL_TYPE_NONE:
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_TCP:
			nType = UTILS::NET::TRANS_PROTOCOL_TYPE_TCP;
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_UDP:
			nType = UTILS::NET::TRANS_PROTOCOL_TYPE_UDP;
			break;
		default:
			break;
		}
		if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_NONE){
			return false;
		}
		int err = m_Net.OpenSocket(nType, ip, port);
		if (err != 0){
			return false;
		}
		m_Net.SetSktNoBlock();
		if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_TCP){
			m_Net.SetTcpNoDelay();
		}
		m_Net.SetSktReuseAddr(true);
		return true;
	}

	void CNetServer::RegisterClientCallBack(std::function<void(UTIL_NET::CNetClient* cNewClient)> cb)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		m_cbNewClientCallback = cb;
	}

	void CNetServer::RegisterClientCallBackEx(std::function<void(SOCKET skt)> cb)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		m_cbNewClientCallbackEx = cb;
	}

	void CNetServer::CheckThread(BOOL& bRun, CNetCmd* pBuffer)
	{
		UTILS::NET::TRANS_PROTOCOL_TYPE nType = m_Net.GetTransProtocol();
		if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_TCP){
			SOCKET skt = m_Net.Accept();
			if (skt != INVALID_SOCKET) {
				if (m_cbNewClientCallback){
					UTIL_NET::CNetClient* p = new UTIL_NET::CNetClient(UTIL_NET::TRANS_PROTOCOL_TYPE_TCP, skt);
					m_cbNewClientCallback(p);
				}
				if (m_cbNewClientCallbackEx){
					m_cbNewClientCallbackEx(skt);
				}
			}
		}
		else if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_UDP)
		{
			RecvData(pBuffer);
		}
	}

	bool CNetServer::IsError() const
	{
		return m_Net.IsError();
	}

	CNetClient::CNetClient()
		:CNetBase()
	{
	}

	CNetClient::CNetClient(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, SOCKET skt)
		: CNetBase()
	{
		Attach(skt);
	}

	CNetClient::~CNetClient()
	{
	}

	bool CNetClient::Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port, bool bConn /*= true*/)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		UTILS::NET::TRANS_PROTOCOL_TYPE nType = UTILS::NET::TRANS_PROTOCOL_TYPE_NONE;
		switch (nProType)
		{
		case UTIL_NET::TRANS_PROTOCOL_TYPE_NONE:
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_TCP:
			nType = UTILS::NET::TRANS_PROTOCOL_TYPE_TCP;
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_UDP:
			nType = UTILS::NET::TRANS_PROTOCOL_TYPE_UDP;
			break;
		default:
			break;
		}
		if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_NONE){
			return false;
		}
		int err = m_Net.ConnectSocket(nType, ip, port, nullptr, bConn);
		if (err == 0){
			m_Net.SetSktNoBlock();
			if (nType == UTILS::NET::TRANS_PROTOCOL_TYPE_TCP){
				m_Net.SetTcpNoDelay();
			}
			m_Net.SetSktReuseAddr(true);
		}
		return err == 0 ? true : false;
	}

	void CNetClient::Attach(SOCKET skt)
	{
		std::unique_lock<std::mutex> lck(m_lock.GetMutex());
		m_Net.Attach(skt);
		m_Net.SetTcpNoDelay();
		m_Net.SetSktReuseAddr(true);
	}

	bool CNetClient::IsError() const
	{
		return m_Net.IsError();
	}

	void CNetClient::CheckThread(BOOL& bRun, CNetCmd* pBuffer)
	{
		RecvData(pBuffer);
	}

	int UTIL_NET_CPPAPI GetSktName(SOCKET skt, char* ip, int len, int* port)
	{
		return UTILS::NET::GetSktName(skt, ip, len, port);
	}

	int UTIL_NET_CPPAPI GetSktNameEx(sockaddr& addr, char* ip, int len, int* port)
	{
		return UTILS::NET::GetSktNameEx(addr, ip, len, port);
	}

	int UTIL_NET_CPPAPI StuffSockAddr(TRANS_PROTOCOL_TYPE nType, char* ip, int port, sockaddr& addr)
	{
		UTILS::NET::TRANS_PROTOCOL_TYPE nType2 = UTILS::NET::TRANS_PROTOCOL_TYPE_NONE;
		switch (nType)
		{
		case UTIL_NET::TRANS_PROTOCOL_TYPE_NONE:
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_TCP:
			nType2 = UTILS::NET::TRANS_PROTOCOL_TYPE_TCP;
			break;
		case UTIL_NET::TRANS_PROTOCOL_TYPE_UDP:
			nType2 = UTILS::NET::TRANS_PROTOCOL_TYPE_UDP;
			break;
		default:
			break;
		}
		return UTILS::NET::StuffSockAddr(nType2, ip, port, addr);
	}

	int UTIL_NET_CPPAPI InitSocket()
	{
		return UTILS::NET::InitSocket();
	}

	int UTIL_NET_CPPAPI UnInitSocket()
	{
		return UTILS::NET::InitSocket();
	}

	int UTIL_NET_CPPAPI CloseSocket(SOCKET skt)
	{
		return UTILS::NET::CloseSocket(skt);
	}

	int UTIL_NET_CPPAPI DuplicateSocket(SOCKET skt, DWORD pid, BYTE* pProtocolInfo, int iBuffLen, int* pDataLen)
	{
		return UTILS::NET::DuplicateSocket(skt, pid, pProtocolInfo, iBuffLen, pDataLen);
	}

	int UTIL_NET_CPPAPI CreateSocketFromDuplicate(BYTE* pProtocolInfo, int iDataLen, SOCKET& skt)
	{
		return UTILS::NET::CreateSocketFromDuplicate(pProtocolInfo, iDataLen, skt);
	}
}