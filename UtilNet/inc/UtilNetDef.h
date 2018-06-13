#ifndef __UTIL_NET_DEF__
#define __UTIL_NET_DEF__

/*
@brief 错误码
*/
enum UTIL_NET_ERROR {
	/**< 没有错误.*/
	UTIL_NET_ERR_SUCCESS = 0,
	/**< 参数错误.*/
	UTIL_NET_ERR_PAR = -1,
	/**< 操作失败.*/
	UTIL_NET_ERR_FAIL = -2,
	/**< 超时.*/
	UTIL_NET_ERR_TIMEOUT = -3,
	/**< 加锁失败.*/
	UTIL_NET_ERR_LOCK = -4,
	/**< 忙.*/
	UTIL_NET_ERR_BUSY = -5,
	/**< 没有初始化.*/
	UTIL_NET_ERR_INIT = -6,
	/**< 分配内存失败.*/
	UTIL_NET_ERR_MALLOC = -7,
};

/*
@brief 网络通信节点信息
*/
typedef struct _UTIL_NET_NODE_INFO
{
	unsigned int uiSize;
	/**< 节点IP地址.*/
	char szIp[64];
	/**< 节点端口.*/
	int iPort;
	/**< 详情.*/
	sockaddr stAddr;
}UTIL_NET_NODE_INFO, *PUTIL_NET_NODE_INFO;

#endif