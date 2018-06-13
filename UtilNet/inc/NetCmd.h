#ifndef __UTIL_NET_CMD__
#define __UTIL_NET_CMD__

#include <UtilNet\inc\UtilNet.h>

namespace UTIL_NET
{
#define DEFAULT_BUFFER_LEN 1024*64

	/**
	@brief ��������ͷ.
	*typedef struct _NET_DATA_HEAD
	*{
	*	�ṹ���С.
	*	unsigned int uiSize;
	*	��ʶ..
	*	unsigned int uiNetFlag;
	*	����ID.
	*	unsigned int uiCmdID;
	*	������.
	*	unsigned int uiMajorrCmd;
	*	������.
	*	unsigned int uiMinorCmd;
	*	ʱ���.
	*	unsigned int uiTimeStamp;
	*	�Զ�������1.
	*	unsigned int uiSelfContext;
	*	�Զ�������2.
	*	unsigned int uiPeerContext;
	*	���.
	*	BOOL bFinish;
	*	������.
	*	int iError;
	*	�������ݳ���.
	*	uint32_t uiDataLen;
	*	��������.
	*	BYTE buff[4];
	*}NET_DATA_HEAD, *PNET_DATA_HEAD;
	*/
	class UTIL_NET_CPPAPI CNetCmd
	{
	public:
		CNetCmd();
		/**
		@brief ���캯��.
		\param uiSize
			��ʼ�������С.�������[DEFAULT_BUFFER_LEN],���ڶ��Ϸ���ָ����С�ڴ�.
		*/
		CNetCmd(unsigned int uiSize);
		~CNetCmd();

	public:

		/**
		@brief ��������ID.
		*/
		void SetCmdID(unsigned int uiID);
		/**
		@brief ��ȡ����ID.
		*/
		unsigned int GetCmdID() const;

		/**
		@brief ����������.
		*/
		void SetMajorCmd(unsigned int uiCmd);
		/**
		@brief ��ȡ������.
		*/
		unsigned int GetMajorCmd() const;

		/**
		@brief ���ô�����.
		*/
		void SetMinorCmd(unsigned int uiCmd);
		/**
		@brief ��ȡ������.
		*/
		unsigned int GetMinorCmd() const;

		/**
		@brief ����ʱ���.
		*/
		void SetTimeStamp(unsigned int uiTimeStamp);
		/**
		@brief ��ȡʱ���.
		*/
		unsigned int GetTimeStamp() const;

		/**
		@brief �����Զ�������.
		\param uiContext
			�Զ�������.
		\param bSelf
			[true]���Ͷ�,[false]���ն�
		*/
		void SetContext(unsigned int uiContext, bool bSelf = true);
		/**
		@brief ��ȡ�Զ�������.
		*/
		unsigned int GetContext(bool bSelf = true) const;

		/**
		@brief ������ɱ�ʶ.
		*/
		void SetFinish(BOOL bFinish);
		/**
		@brief ��ȡ��ɱ�ʶ.
		*/
		BOOL GetFinish() const;

		/**
		@brief ���ô�����.
		*/
		void SetErrorCode(int iError);
		/**
		@brief ��ȡ������.
		*/
		int GetErrorCode() const;

		/**
		@brief ���븽������.
		\param pDataBuff
			�ⲿ���ݻ�����
		\param iDataLen
			���ݴ�С
		\return �������ݴ�С.������ݴ�С�����ڲ��������ݻ�������С,ֱ�ӷ���0
		*/
		int PutData(BYTE* pDataBuff, int iDataLen);
		/**
		@brief ��ȡ��������.
		\param pBuff
			�ⲿ������
		\param iDataLen
			��������С
		\return ��ȡ���ݴ�С
		*/
		int GetData(BYTE* pBuff, int iBuffLen) const;
		/**
		@brief ��ȡ�������ݻ�����.
		\param pBuff
			�ⲿ����ָ���ַ
		*/
		void GetDataPtr(BYTE*& pBuff);
		/**
		@brief ��ȡ�������ݻ�������С.
		\return �����ڲ��ɴ�ŵĸ���������󳤶�.
		*/
		unsigned int GetDataBufferLen();
		/**
		@brief ���ø������ݴ�С.
		\return �μ�[UTIL_NET_ERROR]
		*/
		int SetDataLen(unsigned int iDataLen);

	public:
		CNetCmd(CNetCmd const &) = delete;
		CNetCmd(CNetCmd &&) = delete;
		CNetCmd& operator= (CNetCmd &&) = delete;
		CNetCmd& operator= (CNetCmd const& other);
	private:
		/**
		@brief ��ʼ������.
		\param uiSize
			��ʼ�������С.�������[DEFAULT_BUFFER_LEN],���ڶ��Ϸ���ָ����С�ڴ�.
		*/
		int Init(unsigned int uiSize);

		/**
		@brief �ͷ���Դ.
			����ڶ��Ϸ������ڴ�.���ͷ��ڴ�.
		*/
		void UnInit();

		/**
		@brief ���󿽱�.
		*/
		void Copy(CNetCmd const&);
		/**
		@brief ����/��λ.
		*/
		void Clear();
	private:

		/**< �ڲ���������С.*/
		unsigned int m_uiBuffLen;
		/**< �ڲ�������.*/
		BYTE m_buff[DEFAULT_BUFFER_LEN];
		/**< �ڲ�������ָ��. _buff_len<=1024*64,_ptr_buffָ��_buff,����ָ����ϵ�ַ.*/
		BYTE* m_ptr_buff;
	};
}
#endif