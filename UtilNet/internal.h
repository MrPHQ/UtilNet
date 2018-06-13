#ifndef __UTIL_NET_INTERNAL__
#define __UTIL_NET_INTERNAL__

#define NET_DATA_HEAD_FLAG 0xFF2018FF

/*
@brief ��������ͷ.
*/
typedef struct _NET_DATA_HEAD
{
	/**< �ṹ���С.*/
	unsigned int uiSize;
	/**< ��ʶ.�μ�[NET_DATA_HEAD_FLAG].*/
	unsigned int uiNetFlag;
	/**< ����ID.*/
	unsigned int uiCmdID;
	/**< ������.�μ�[INC_PATCH_NET_CMD_TYPE].*/
	unsigned int uiMajorrCmd;
	/**< ������.�μ�[INC_PATCH_NET_MINOR_CMD_TYPE]*/
	unsigned int uiMinorCmd;
	/**< ʱ���.*/
	unsigned int uiTimeStamp;
	/**< �Զ�������1.*/
	unsigned int uiSelfContext;
	/**< �Զ�������2.*/
	unsigned int uiPeerContext;
	/**< ���.*/
	BOOL bFinish;
	/**< ��ֹ����.*/
	BOOL bSuspend;
	/**< ������.*/
	int iError;
	/**< �������ݳ���.*/
	uint32_t uiDataLen;
	/**< ��������.*/
	BYTE buff[4];
}NET_DATA_HEAD, *PNET_DATA_HEAD;


#endif