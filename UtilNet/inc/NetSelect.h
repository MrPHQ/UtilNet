#ifndef __UTIL_NET_SELECT__
#define __UTIL_NET_SELECT__

#include <UtilNet\inc\UtilNet.h>
#include <UtilNet\inc\NetCmd.h>
namespace UTIL_NET
{

	/**
	@brief ��������ͷ.
	*/
	class UTIL_NET_CPPAPI CNetBase
	{
	public:
		CNetBase();
		~CNetBase();

	public:
		/**
		@brief ע��״̬�ص�����.
		\param cb
			�ص���������.
		*/
		void RegisterStatusCallBack(std::function<void(BOOL bStatus)> cb);

		/**
		@brief ע��������ݻص�����.
		\param cb
			�ص���������.
		*/
		void RegisterDataCallBack(std::function<void(CNetCmd*)> cb);
		
		/**
		@brief ��������.
		\param pDataBuff
			���������ݻ�����
		\param iDataLen
			���������ݴ�С.
		*/
		int Send(CNetCmd*);

		/**
		@brief ��������.
		\param pDataBuff
			���������ݻ�����
		\param iDataLen
			���������ݴ�С.
		\param cb
			��ȡʣ�෢�����ݻص�����.
		*/
		int Send(CNetCmd*, std::function<int(CNetCmd*&, int)> cb);

		/**
		@brief �Ƿ��ʹ���.
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
		@brief �Ƿ��ʹ���.
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
		@brief �Ƿ��ʹ���.
		\return
		*/
		bool IsError();
	};
}
#endif