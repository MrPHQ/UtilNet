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

#endif