#ifndef __UTIL_NET_INTERNAL__
#define __UTIL_NET_INTERNAL__

#define NET_DATA_HEAD_FLAG 0xFF2018FF

/*
@brief 网络数据头.
*/
typedef struct _NET_DATA_HEAD
{
	/**< 结构体大小.*/
	unsigned int uiSize;
	/**< 标识.参见[NET_DATA_HEAD_FLAG].*/
	unsigned int uiNetFlag;
	/**< 命令ID.*/
	unsigned int uiCmdID;
	/**< 主命令.参见[INC_PATCH_NET_CMD_TYPE].*/
	unsigned int uiMajorrCmd;
	/**< 从命令.参见[INC_PATCH_NET_MINOR_CMD_TYPE]*/
	unsigned int uiMinorCmd;
	/**< 时间戳.*/
	unsigned int uiTimeStamp;
	/**< 自定义数据1.*/
	unsigned int uiSelfContext;
	/**< 自定义数据2.*/
	unsigned int uiPeerContext;
	/**< 完成.*/
	BOOL bFinish;
	/**< 终止任务.*/
	BOOL bSuspend;
	/**< 错误码.*/
	int iError;
	/**< 附带数据长度.*/
	uint32_t uiDataLen;
	/**< 附带数据.*/
	BYTE buff[4];
}NET_DATA_HEAD, *PNET_DATA_HEAD;


#endif