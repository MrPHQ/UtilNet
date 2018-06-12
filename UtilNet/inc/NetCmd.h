#ifndef __UTIL_NET_CMD__
#define __UTIL_NET_CMD__

#include <UtilNet\inc\UtilNet.h>
namespace UTIL_NET
{
#define DEFAULT_BUFFER_LEN 1024*64

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

	private:
		/**< ��ʶ.�μ�[NET_DATA_HEAD_FLAG].*/
		unsigned int m_uiNetFlag;
		/**< ����ID.*/
		unsigned int m_uiCmdID;
		/**< ������.�μ�[INC_PATCH_NET_CMD_TYPE].*/
		unsigned int m_uiMajorrCmd;
		/**< ������.�μ�[INC_PATCH_NET_MINOR_CMD_TYPE]*/
		unsigned int m_uiMinorCmd;
		/**< ʱ���.*/
		unsigned int m_uiTimeStamp;
		/**< �Զ�������1.*/
		unsigned int m_uiSelfContext;
		/**< �Զ�������2.*/
		unsigned int m_uiPeerContext;
		/**< ���.*/
		BOOL m_bFinish;
		/**< ������.*/
		int m_iError;

		/**< �ڲ���������С.*/
		unsigned int m_uiBuffLen;
		/**< �ڲ�������.*/
		BYTE m_buff[DEFAULT_BUFFER_LEN];
		/**< �ڲ�������ָ��. _buff_len<=1024*64,_ptr_buffָ��_buff,����ָ����ϵ�ַ.*/
		BYTE* m_ptr_buff;
	};
}
#endif