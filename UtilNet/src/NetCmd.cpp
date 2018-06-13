#include "../stdafx.h"
#include "../inc/NetCmd.h"

namespace UTIL_NET
{
	CNetCmd::CNetCmd()
		:m_nBuffMode(NET_CMD_BUFF_MODE_NONE), m_uiBuffLen(0), m_ptr_buff(nullptr)
	{
		Init(0);
	}

	CNetCmd::CNetCmd(unsigned int uiSize)
		: m_nBuffMode(NET_CMD_BUFF_MODE_NONE), m_uiBuffLen(0), m_ptr_buff(nullptr)
	{
		Init(uiSize);
	}

	CNetCmd::~CNetCmd()
	{
		UnInit();
	}

	int CNetCmd::Init(unsigned int uiSize)
	{
		if (uiSize > DEFAULT_BUFFER_LEN) {
			m_uiBuffLen = PAD_SIZE(uiSize);
			m_ptr_buff = new BYTE[m_uiBuffLen];
			if (nullptr == m_ptr_buff) {
				m_uiBuffLen = 0;
			}
		}
		else {
			m_ptr_buff = m_buff;
			m_uiBuffLen = DEFAULT_BUFFER_LEN;
		}
		if (m_ptr_buff == nullptr) {
			return UTIL_NET_ERR_MALLOC;
		}
		Clear();
		m_nBuffMode = NET_CMD_BUFF_MODE_INTERNAL;
		return UTIL_NET_ERR_SUCCESS;
	}

	void CNetCmd::UnInit()
	{
		if ((m_nBuffMode == NET_CMD_BUFF_MODE_INTERNAL) && (m_uiBuffLen > DEFAULT_BUFFER_LEN) && (m_ptr_buff != nullptr)){
			delete[] m_ptr_buff;
			m_ptr_buff = nullptr;
			m_uiBuffLen = 0;
		}
	}

	void CNetCmd::Copy(CNetCmd const& other)
	{
		if (this == &other) {
			return;
		}
		if ((m_uiBuffLen > DEFAULT_BUFFER_LEN) && (m_ptr_buff != nullptr)){
			delete[] m_ptr_buff;
			m_ptr_buff = nullptr;
		}
		m_uiBuffLen = other.m_uiBuffLen;
		if (m_uiBuffLen > DEFAULT_BUFFER_LEN){
			m_uiBuffLen = PAD_SIZE(m_uiBuffLen);
			m_ptr_buff = new BYTE[m_uiBuffLen];
			if (nullptr == m_ptr_buff) {
				m_uiBuffLen = 0;
			}
		}
		else{
			m_ptr_buff = m_buff;
			m_uiBuffLen = DEFAULT_BUFFER_LEN;
		}
		if (m_ptr_buff == nullptr || m_uiBuffLen <= 0){
			return;
		}
		if (other.m_ptr_buff == nullptr){
			return;
		}

		PNET_DATA_HEAD info = (PNET_DATA_HEAD)other.m_ptr_buff;
		memcpy(m_ptr_buff, other.m_ptr_buff, min(m_uiBuffLen, info->uiSize + info->uiDataLen));
	}

	void CNetCmd::Clear()
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		ZeroMemory(pInfo, sizeof(NET_DATA_HEAD));
		pInfo->uiSize = sizeof(NET_DATA_HEAD);
		pInfo->uiNetFlag = NET_DATA_HEAD_FLAG;
	}

	void CNetCmd::SetMajorCmd(unsigned int uiCmd)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->uiMajorrCmd = uiCmd;
	}

	unsigned int CNetCmd::GetMajorCmd() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->uiMajorrCmd;
	}

	void CNetCmd::SetMinorCmd(unsigned int uiCmd)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->uiMinorCmd = uiCmd;
	}

	unsigned int CNetCmd::GetMinorCmd() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->uiMinorCmd;
	}

	void CNetCmd::SetCmdID(unsigned int uiID)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->uiCmdID = uiID;
	}

	unsigned int CNetCmd::GetCmdID() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->uiCmdID;
	}

	void CNetCmd::SetTimeStamp(unsigned int uiTimeStamp)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->uiTimeStamp = uiTimeStamp;
	}

	unsigned int CNetCmd::GetTimeStamp() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->uiTimeStamp;
	}

	void CNetCmd::SetContext(unsigned int uiContext, bool bSelf/* = true*/)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		unsigned int* pInt = nullptr;
		if (bSelf) {
			pInt = &pInfo->uiSelfContext;
		}
		else {
			pInt = &pInfo->uiPeerContext;
		}
		*pInt = uiContext;
	}

	unsigned int CNetCmd::GetContext(bool bSelf/* = true*/) const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		unsigned int* pInt = nullptr;
		if (bSelf) {
			pInt = &pInfo->uiSelfContext;
		}
		else {
			pInt = &pInfo->uiPeerContext;
		}
		return *pInt;
	}

	void CNetCmd::SetFinish(BOOL bFinish)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->bFinish = bFinish;
	}

	BOOL CNetCmd::GetFinish() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->bFinish;
	}

	void CNetCmd::SetSuspend(BOOL bSuspend)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->bSuspend = bSuspend;
	}

	BOOL CNetCmd::GetSuspend() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->bSuspend;
	}

	void CNetCmd::SetErrorCode(int iError)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pInfo->iError = iError;
	}

	int CNetCmd::GetErrorCode() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->iError;
	}

	int CNetCmd::PutData(BYTE* pDataBuff, int iDataLen)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		if (iDataLen > (int)(m_uiBuffLen - sizeof(NET_DATA_HEAD))){
			return 0;
		}
		memcpy(pInfo->buff, pDataBuff, iDataLen);
		return iDataLen;
	}

	int CNetCmd::GetData(BYTE* pBuff, int iBuffLen) const
	{
		assert(m_ptr_buff != nullptr);
		if (pBuff == nullptr){
			return UTIL_NET_ERR_PAR;
		}
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		int len = min((unsigned int)iBuffLen, pInfo->uiDataLen);
		memcpy(pBuff, pInfo->buff, len);
		return len;
	}

	void CNetCmd::GetDataPtr(BYTE*& pBuff)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		pBuff = pInfo->buff;
	}

	BYTE* CNetCmd::GetDataPtr()
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->buff;
	}

	unsigned int CNetCmd::GetDataBufferLen()
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return m_uiBuffLen - sizeof(NET_DATA_HEAD);
	}

	int CNetCmd::SetDataLen(unsigned int iDataLen)
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		if (iDataLen > (m_uiBuffLen - sizeof(NET_DATA_HEAD))){
			return UTIL_NET_ERR_PAR;
		}
		pInfo->uiDataLen = iDataLen;
		return UTIL_NET_ERR_SUCCESS;
	}

	unsigned int CNetCmd::GetDataLen() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		return pInfo->uiDataLen;
	}

	unsigned int CNetCmd::GetHeadLen() const
	{
		return sizeof(NET_DATA_HEAD);
	}

	CNetCmd& CNetCmd::operator= (CNetCmd const& other)
	{
		Copy(other);
		return *this;
	}

	BYTE* CNetCmd::GetBufferPtr()
	{
		assert(m_ptr_buff != nullptr);
		return m_ptr_buff;
	}

	unsigned int CNetCmd::GetDataTotalLen() const
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD pInfo = (PNET_DATA_HEAD)m_ptr_buff;
		if (pInfo->uiSize + pInfo->uiDataLen > m_uiBuffLen){
			return 0;
		}
		return pInfo->uiSize + pInfo->uiDataLen;
	}

	unsigned int CNetCmd::GetBufferLen() const
	{
		assert(m_ptr_buff != nullptr);
		return m_uiBuffLen;
	}

	void CNetCmd::Attach(BYTE* pBuff, int iBuffLen)
	{
		if (nullptr == pBuff){
			return;
		}
		m_nBuffMode = NET_CMD_BUFF_MODE_EXTERNAL;
		m_ptr_buff = pBuff;
		m_uiBuffLen = iBuffLen;
	}

	bool CNetCmd::Verity()
	{
		assert(m_ptr_buff != nullptr);
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)m_ptr_buff;
		if (info->uiSize != sizeof(NET_DATA_HEAD)) {
			return false;
		}

		if (info->uiNetFlag != NET_DATA_HEAD_FLAG){
			return false;
		}
		return true;
	}
}