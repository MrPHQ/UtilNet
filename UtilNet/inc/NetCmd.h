#ifndef __UTIL_NET_CMD__
#define __UTIL_NET_CMD__

#include <UtilNet\inc\UtilNet.h>

namespace UTIL_NET
{
#define DEFAULT_BUFFER_LEN 1024*64

	/**
	@brief 网络命令头.
	*typedef struct _NET_DATA_HEAD
	*{
	*	结构体大小.
	*	unsigned int uiSize;
	*	标识..
	*	unsigned int uiNetFlag;
	*	命令ID.
	*	unsigned int uiCmdID;
	*	主命令.
	*	unsigned int uiMajorrCmd;
	*	从命令.
	*	unsigned int uiMinorCmd;
	*	时间戳.
	*	unsigned int uiTimeStamp;
	*	自定义数据1.
	*	unsigned int uiSelfContext;
	*	自定义数据2.
	*	unsigned int uiPeerContext;
	*	完成.
	*	BOOL bFinish;
	*	错误码.
	*	int iError;
	*	附带数据长度.
	*	uint32_t uiDataLen;
	*	附带数据.
	*	BYTE buff[4];
	*}NET_DATA_HEAD, *PNET_DATA_HEAD;
	*/
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
		@brief 设置命令ID.
		*/
		void SetCmdID(unsigned int uiID);
		/**
		@brief 获取命令ID.
		*/
		unsigned int GetCmdID() const;

		/**
		@brief 设置主命令.
		*/
		void SetMajorCmd(unsigned int uiCmd);
		/**
		@brief 获取主命令.
		*/
		unsigned int GetMajorCmd() const;

		/**
		@brief 设置从命令.
		*/
		void SetMinorCmd(unsigned int uiCmd);
		/**
		@brief 获取从命令.
		*/
		unsigned int GetMinorCmd() const;

		/**
		@brief 设置时间戳.
		*/
		void SetTimeStamp(unsigned int uiTimeStamp);
		/**
		@brief 获取时间戳.
		*/
		unsigned int GetTimeStamp() const;

		/**
		@brief 设置自定义数据.
		\param uiContext
			自定义数据.
		\param bSelf
			[true]发送端,[false]接收端
		*/
		void SetContext(unsigned int uiContext, bool bSelf = true);
		/**
		@brief 获取自定义数据.
		*/
		unsigned int GetContext(bool bSelf = true) const;

		/**
		@brief 设置完成标识.
		*/
		void SetFinish(BOOL bFinish);
		/**
		@brief 获取完成标识.
		*/
		BOOL GetFinish() const;

		/**
		@brief 设置错误码.
		*/
		void SetErrorCode(int iError);
		/**
		@brief 获取错误码.
		*/
		int GetErrorCode() const;

		/**
		@brief 输入附加数据.
		\param pDataBuff
			外部数据缓存区
		\param iDataLen
			数据大小
		\return 输入数据大小.如果数据大小大于内部附加数据缓存区大小,直接返回0
		*/
		int PutData(BYTE* pDataBuff, int iDataLen);
		/**
		@brief 获取附加数据.
		\param pBuff
			外部缓存区
		\param iDataLen
			缓存区大小
		\return 读取数据大小
		*/
		int GetData(BYTE* pBuff, int iBuffLen) const;
		/**
		@brief 获取附加数据缓存区.
		\param pBuff
			外部数据指针地址
		*/
		void GetDataPtr(BYTE*& pBuff);
		/**
		@brief 获取附加数据缓存区大小.
		\return 返回内部可存放的附加数据最大长度.
		*/
		unsigned int GetDataBufferLen();
		/**
		@brief 设置附加数据大小.
		\return 参见[UTIL_NET_ERROR]
		*/
		int SetDataLen(unsigned int iDataLen);

	public:
		CNetCmd(CNetCmd const &) = delete;
		CNetCmd(CNetCmd &&) = delete;
		CNetCmd& operator= (CNetCmd &&) = delete;
		CNetCmd& operator= (CNetCmd const& other);
	private:
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

		/**
		@brief 对象拷贝.
		*/
		void Copy(CNetCmd const&);
		/**
		@brief 清理/复位.
		*/
		void Clear();
	private:

		/**< 内部缓存区大小.*/
		unsigned int m_uiBuffLen;
		/**< 内部缓存区.*/
		BYTE m_buff[DEFAULT_BUFFER_LEN];
		/**< 内部缓存区指针. _buff_len<=1024*64,_ptr_buff指向_buff,否则指向堆上地址.*/
		BYTE* m_ptr_buff;
	};
}
#endif