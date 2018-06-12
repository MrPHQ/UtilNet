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
		@brief 构造函数.
		\param uiSize
			初始化缓存大小.如果大于[DEFAULT_BUFFER_LEN],将在堆上分配指定大小内存.
		*/
		CNetCmd(unsigned int uiSize);
		~CNetCmd();

	public:
		/**
		@brief 初始化函数.
		\param uiSize
			初始化缓存大小.如果大于[DEFAULT_BUFFER_LEN],将在堆上分配指定大小内存.
		*/
		int Init(unsigned int uiSize);
		/**
		@brief 释放资源.
			如果在堆上分配了内存.将释放内存.
		*/
		void UnInit();

	private:
		/**< 标识.参见[NET_DATA_HEAD_FLAG].*/
		unsigned int m_uiNetFlag;
		/**< 命令ID.*/
		unsigned int m_uiCmdID;
		/**< 主命令.参见[INC_PATCH_NET_CMD_TYPE].*/
		unsigned int m_uiMajorrCmd;
		/**< 从命令.参见[INC_PATCH_NET_MINOR_CMD_TYPE]*/
		unsigned int m_uiMinorCmd;
		/**< 时间戳.*/
		unsigned int m_uiTimeStamp;
		/**< 自定义数据1.*/
		unsigned int m_uiSelfContext;
		/**< 自定义数据2.*/
		unsigned int m_uiPeerContext;
		/**< 完成.*/
		BOOL m_bFinish;
		/**< 错误码.*/
		int m_iError;

		/**< 内部缓存区大小.*/
		unsigned int m_uiBuffLen;
		/**< 内部缓存区.*/
		BYTE m_buff[DEFAULT_BUFFER_LEN];
		/**< 内部缓存区指针. _buff_len<=1024*64,_ptr_buff指向_buff,否则指向堆上地址.*/
		BYTE* m_ptr_buff;
	};
}
#endif