#ifndef __UTIL_NET_SELECT__
#define __UTIL_NET_SELECT__

#include <UtilNet\inc\UtilNet.h>
#include <UtilNet\inc\NetCmd.h>
namespace UTIL_NET
{
	enum TRANS_PROTOCOL_TYPE
	{
		TRANS_PROTOCOL_TYPE_NONE = 0,
		/**< 传输协议使用TCP.*/
		TRANS_PROTOCOL_TYPE_TCP = 1,
		/**< 传输协议使用UDP.*/
		TRANS_PROTOCOL_TYPE_UDP = 2
	};

#define DEFAULT_NET_DATA_BUFFER_LEN 1024*1024


	/**
	@brief [select]模型网络通信基类.
		线程安全.
	*/
	class UTIL_NET_CPPAPI CNetBase
	{
	public:
		CNetBase();
		~CNetBase();

	public:
		/**
		@brief 初始化.
		\param uiBufferSize
			网络数据缓存区大小,
		\return [UTIL_NET_ERROR]
		*/
		virtual int Init(unsigned int uiBufferSize = DEFAULT_NET_DATA_BUFFER_LEN);
		/**
		@brief 释放资源.
		*/
		virtual void UnInit();

		/**
		@brief 断开.
		*/
		virtual void Close();

		/**
		@brief 注册状态回调函数.
			间隔5s检查一次网络状态
		\param cb
			回调函数声明.
		*/
		void RegisterStatusCallBack(std::function<void(BOOL bStatus)> cb);

		/**
		@brief 注册接收数据回调函数.
		\param cb
			回调函数声明.
		*/
		void RegisterDataCallBack(std::function<void(CNetCmd*, PUTIL_NET_NODE_INFO form)> cb);

		/**
		@brief 发送数据.
			注意:TCP/UDP都可以调用,UDP调用前需显示的连接UDP服务器
		\param pDataBuff
			待发送数据缓存区
		\param bLocked
			是否已经加锁, 注意:外部调用接口,该值必须是 [false]
		\return 已发送数据长度
		*/
		int Send(CNetCmd* pDataBuff, unsigned int uiTimeOut = 5000, bool bLocked = false);

		/**
		@brief 发送数据.
			注意:TCP/UDP都可以调用,UDP调用前需显示的连接UDP服务器
		\param pDataBuff
			待发送数据缓存区
		\param cb
			读取剩余发送数据回调函数.回调返回0,则继续发送,否则停止发送
		\return 已发送数据长度
		*/
		int SendEx(CNetCmd* pDataBuff, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut = 5000);

		/**
		@brief 发送数据.
			注意:UDP专用
		\param pDataBuff
			待发送数据缓存区
		\param to
			对端地址信息
		\param tolen
			地址信息大小
		\param bLocked
			是否已经加锁, 注意:外部调用接口,该值必须是 [false]
		\return 已发送数据长度
		*/
		int SendToUDP(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, unsigned int uiTimeOut = 5000, bool bLocked = false);

		/**
		@brief 发送数据.
			注意:UDP专用
		\param pDataBuff
			待发送数据缓存区
		\param to
			对端地址信息
		\param tolen
			地址信息大小
		\param cb
			读取剩余发送数据回调函数.回调返回0,则继续发送,否则停止发送
		\return 已发送数据长度
		*/
		int SendToUDPEx(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut = 5000);

		/**
		@brief 是否发送错误.
		\return 
		*/
		virtual bool IsError() const = 0;

	protected:
		/**
		@brief 获取通信实例.
		\return 
		*/
		virtual UTILS::NET::SELECT::CNet& GetObjectIns() = 0;
		/**
		@brief 读取网络数据.
		\return
		*/
		void RecvData(CNetCmd* pBuffer);
	private:

		/**
		@brief 网络是否中断.
		\return
		*/
		bool CheckStatus();

		/**
		@brief 监听/数据接收线程.
		\return
		*/
		virtual void CheckThread(BOOL& bRun, CNetCmd* pBuffer);
	protected:
		/**< 资源锁.*/
		UTILS::CLock m_lock;
		/**< 环形缓存区.用于接收网络数据.*/
		UTILS::BUFFER::CRingBuffer m_buff;
		/**< 网络数据回调.*/
		std::function<void(CNetCmd*, PUTIL_NET_NODE_INFO form)> m_cbDataCallback;
	private:
		/**< 是否初始化标识.*/
		BOOL m_bInit;
		/**< 监听/网络数据接收线程.*/
		UTILS::CThreadBox m_tbNet;
		/**< 网络通信状态回调.*/
		std::function<void(BOOL bStatus)> m_cbStatusCallback;
	};

	/**
	@brief [select]模型网络服务器端.
	*/
	class CNetClient;
	class UTIL_NET_CPPAPI CNetServer
		: public CNetBase
	{
	public:
		CNetServer();
		~CNetServer();

	public:

		/**
		@brief 打开网络连接.
		\param nProType
			网络传输协议.
		\param ip
			IP地址
		\param port
			端口
		*/
		bool Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port);

		/**
		@brief 注册新客户端连接回调函数.
		\param cb
			回调函数声明.
		*/
		void RegisterClientCallBack(std::function<void(UTIL_NET::CNetClient* cNewClient)> cb);
		void RegisterClientCallBackEx(std::function<void(SOCKET skt)> cb);

		/**
		@brief 是否发送错误.
		\return 
		*/
		bool IsError() const;
	protected:
		/**
		@brief 监听线程.
		\return
		*/
		void CheckThread(BOOL& bRun, CNetCmd* pBuffer);

		/**
		@brief 获取通信实例.
		*/
		UTILS::NET::SELECT::CNet& GetObjectIns() { return m_Net; }
	private:
		/**< 网络通信实例.*/
		UTILS::NET::SELECT::CNetServer m_Net;
		/**< 客户端连接回调.*/
		std::function<void(UTIL_NET::CNetClient* cNewClient)> m_cbNewClientCallback;
		std::function<void(SOCKET skt)> m_cbNewClientCallbackEx;
	};

	/**
	@brief [select]模型网络客户端.
	*/
	class UTIL_NET_CPPAPI CNetClient
		: public CNetBase
	{
	public:
		CNetClient();
		CNetClient(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, SOCKET skt);
		~CNetClient();

	public:

		/**
		@brief 打开网络连接.
		\param nProType
			网络传输协议.
		\param ip
			IP地址
		\param port
			端口
		\param bConn
			是否连接,如果[nProType=TRANS_PROTOCOL_TYPE_UDP]可不连接
		*/
		bool Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port, bool bConn = true);

		/*
		@brief 关联套接字.
			接口内部自动设置为 [TRANS_PROTOCOL_TYPE_TCP]
		\param skt
			有效套接字
		*/
		void Attach(SOCKET skt);

		/**
		@brief 是否发送错误.
		\return
		*/
		bool IsError() const;
	protected:
		/**
		@brief 获取通信实例.
		*/
		UTILS::NET::SELECT::CNet& GetObjectIns() { return m_Net; }

		/**
		@brief 数据接收线程.
		\return
		*/
		void CheckThread(BOOL& bRun, CNetCmd* pBuffer);
	private:
		/**< 网络通信实例.*/
		UTILS::NET::SELECT::CNetClient m_Net;
	};

	/*
	@brief 检索套接字的本地名称.
	\param skt
		有效套接字
	\param ip
		存放IP地址的缓存区
	\param len
		缓存区大小
	\param port
		端口
	*/
	int UTIL_NET_CPPAPI GetSktName(SOCKET skt, char* ip, int len, int* port);
	
	/*
	@brief 获取节点的IP、端口.
	\param addr
		节点地址信息
	\param ip
		ip缓存区
	param port
		port缓存区
	*/
	int UTIL_NET_CPPAPI GetSktNameEx(sockaddr& addr, char* ip, int len, int* port);

	/*
	@brief 填充[sockaddr]结构体.
	\param ip
		存放IP地址的缓存区
	\param port
		端口
	param addr
		输出[sockaddr]结构体.缓存区
	*/
	int UTIL_NET_CPPAPI StuffSockAddr(TRANS_PROTOCOL_TYPE nType, char* ip, int port, sockaddr& addr);
}
#endif