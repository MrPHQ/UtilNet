#ifndef __UTIL_NET_DEF__
#define __UTIL_NET_DEF__

/*
@brief ������
*/
enum UTIL_NET_ERROR {
	/**< û�д���.*/
	UTIL_NET_ERR_SUCCESS = 0,
	/**< ��������.*/
	UTIL_NET_ERR_PAR = -1,
	/**< ����ʧ��.*/
	UTIL_NET_ERR_FAIL = -2,
	/**< ��ʱ.*/
	UTIL_NET_ERR_TIMEOUT = -3,
	/**< ����ʧ��.*/
	UTIL_NET_ERR_LOCK = -4,
	/**< æ.*/
	UTIL_NET_ERR_BUSY = -5,
	/**< û�г�ʼ��.*/
	UTIL_NET_ERR_INIT = -6,
	/**< �����ڴ�ʧ��.*/
	UTIL_NET_ERR_MALLOC = -7,
};

#endif