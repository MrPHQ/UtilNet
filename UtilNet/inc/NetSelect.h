#ifndef __UTIL_NET_SELECT__
#define __UTIL_NET_SELECT__

#include <UtilNet\inc\UtilNet.h>
#include <UtilNet\inc\NetCmd.h>
namespace UTIL_NET
{

	/**
	@brief 网络命令头.
	*/
	class UTIL_NET_CPPAPI CNetBase
	{
	public:
		CNetBase();
		~CNetBase();

	public:
		/**
		@brief 注册状态回调函数.
		\param cb
			回调函数声明.
		*/
		void RegisterStatusCallBack(std::function<void(BOOL bStatus)> cb);

		/**
		@brief 注册接收数据回调函数.
		\param cb
			回调函数声明.
		*/
		void RegisterDataCallBack(std::function<void(CNetCmd*)> cb);
		
		/**
		@brief 发送数据.
		\param pDataBuff
			待发送数据缓存区
		\param iDataLen
			待发送数据大小.
		*/
		int Send(CNetCmd*);

		/**
		@brief 发送数据.
		\param pDataBuff
			待发送数据缓存区
		\param iDataLen
			待发送数据大小.
		\param cb
			读取剩余发送数据回调函数.
		*/
		int Send(CNetCmd*, std::function<int(CNetCmd*&, int)> cb);

		/**
		@brief 是否发送错误.
		\return 
		*/
		virtual bool IsError() =0;
	};

	class UTIL_NET_CPPAPI CNetServer
		: public CNetBase
	{
	public:
		CNetServer();
		~CNetServer();

	public:

		/**
		@brief 是否发送错误.
		\return 
		*/
		bool IsError();
	};

	class UTIL_NET_CPPAPI CNetClient
		: public CNetBase
	{
	public:
		CNetClient();
		~CNetClient();

	public:

		/**
		@brief 是否发送错误.
		\return
		*/
		bool IsError();
	};
}
#endif