#ifndef __UTIL_NET_SELECT__
#define __UTIL_NET_SELECT__

#include <UtilNet\inc\UtilNet.h>
#include <UtilNet\inc\NetCmd.h>
namespace UTIL_NET
{
	enum TRANS_PROTOCOL_TYPE
	{
		TRANS_PROTOCOL_TYPE_NONE = 0,
		/**< ����Э��ʹ��TCP.*/
		TRANS_PROTOCOL_TYPE_TCP = 1,
		/**< ����Э��ʹ��UDP.*/
		TRANS_PROTOCOL_TYPE_UDP = 2
	};

#define DEFAULT_NET_DATA_BUFFER_LEN 1024*1024


	/**
	@brief [select]ģ������ͨ�Ż���.
		�̰߳�ȫ.
	*/
	class UTIL_NET_CPPAPI CNetBase
	{
	public:
		CNetBase();
		~CNetBase();

	public:
		/**
		@brief ��ʼ��.
		\param uiBufferSize
			�������ݻ�������С,
		\return [UTIL_NET_ERROR]
		*/
		virtual int Init(unsigned int uiBufferSize = DEFAULT_NET_DATA_BUFFER_LEN);
		/**
		@brief �ͷ���Դ.
		*/
		virtual void UnInit();

		/**
		@brief �Ͽ�.
		*/
		virtual void Close();

		/**
		@brief ע��״̬�ص�����.
			���5s���һ������״̬
		\param cb
			�ص���������.
		*/
		void RegisterStatusCallBack(std::function<void(BOOL bStatus)> cb);

		/**
		@brief ע��������ݻص�����.
		\param cb
			�ص���������.
		*/
		void RegisterDataCallBack(std::function<void(CNetCmd*, PUTIL_NET_NODE_INFO form)> cb);

		/**
		@brief ��������.
			ע��:TCP/UDP�����Ե���,UDP����ǰ����ʾ������UDP������
		\param pDataBuff
			���������ݻ�����
		\param bLocked
			�Ƿ��Ѿ�����, ע��:�ⲿ���ýӿ�,��ֵ������ [false]
		\return �ѷ������ݳ���
		*/
		int Send(CNetCmd* pDataBuff, unsigned int uiTimeOut = 5000, bool bLocked = false);

		/**
		@brief ��������.
			ע��:TCP/UDP�����Ե���,UDP����ǰ����ʾ������UDP������
		\param pDataBuff
			���������ݻ�����
		\param cb
			��ȡʣ�෢�����ݻص�����.�ص�����0,���������,����ֹͣ����
		\return �ѷ������ݳ���
		*/
		int SendEx(CNetCmd* pDataBuff, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut = 5000);

		/**
		@brief ��������.
			ע��:UDPר��
		\param pDataBuff
			���������ݻ�����
		\param to
			�Զ˵�ַ��Ϣ
		\param tolen
			��ַ��Ϣ��С
		\param bLocked
			�Ƿ��Ѿ�����, ע��:�ⲿ���ýӿ�,��ֵ������ [false]
		\return �ѷ������ݳ���
		*/
		int SendToUDP(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, unsigned int uiTimeOut = 5000, bool bLocked = false);

		/**
		@brief ��������.
			ע��:UDPר��
		\param pDataBuff
			���������ݻ�����
		\param to
			�Զ˵�ַ��Ϣ
		\param tolen
			��ַ��Ϣ��С
		\param cb
			��ȡʣ�෢�����ݻص�����.�ص�����0,���������,����ֹͣ����
		\return �ѷ������ݳ���
		*/
		int SendToUDPEx(CNetCmd* pDataBuff, struct sockaddr& to, int tolen, std::function<int(CNetCmd*&)> cb, int* err, unsigned int uiTimeOut = 5000);

		/**
		@brief �Ƿ��ʹ���.
		\return 
		*/
		virtual bool IsError() const = 0;

	protected:
		/**
		@brief ��ȡͨ��ʵ��.
		\return 
		*/
		virtual UTILS::NET::SELECT::CNet& GetObjectIns() = 0;
		/**
		@brief ��ȡ��������.
		\return
		*/
		void RecvData(CNetCmd* pBuffer);
	private:

		/**
		@brief �����Ƿ��ж�.
		\return
		*/
		bool CheckStatus();

		/**
		@brief ����/���ݽ����߳�.
		\return
		*/
		virtual void CheckThread(BOOL& bRun, CNetCmd* pBuffer);
	protected:
		/**< ��Դ��.*/
		UTILS::CLock m_lock;
		/**< ���λ�����.���ڽ�����������.*/
		UTILS::BUFFER::CRingBuffer m_buff;
		/**< �������ݻص�.*/
		std::function<void(CNetCmd*, PUTIL_NET_NODE_INFO form)> m_cbDataCallback;
	private:
		/**< �Ƿ��ʼ����ʶ.*/
		BOOL m_bInit;
		/**< ����/�������ݽ����߳�.*/
		UTILS::CThreadBox m_tbNet;
		/**< ����ͨ��״̬�ص�.*/
		std::function<void(BOOL bStatus)> m_cbStatusCallback;
	};

	/**
	@brief [select]ģ�������������.
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
		@brief ����������.
		\param nProType
			���紫��Э��.
		\param ip
			IP��ַ
		\param port
			�˿�
		*/
		bool Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port);

		/**
		@brief ע���¿ͻ������ӻص�����.
		\param cb
			�ص���������.
		*/
		void RegisterClientCallBack(std::function<void(UTIL_NET::CNetClient* cNewClient)> cb);
		void RegisterClientCallBackEx(std::function<void(SOCKET skt)> cb);

		/**
		@brief �Ƿ��ʹ���.
		\return 
		*/
		bool IsError() const;
	protected:
		/**
		@brief �����߳�.
		\return
		*/
		void CheckThread(BOOL& bRun, CNetCmd* pBuffer);

		/**
		@brief ��ȡͨ��ʵ��.
		*/
		UTILS::NET::SELECT::CNet& GetObjectIns() { return m_Net; }
	private:
		/**< ����ͨ��ʵ��.*/
		UTILS::NET::SELECT::CNetServer m_Net;
		/**< �ͻ������ӻص�.*/
		std::function<void(UTIL_NET::CNetClient* cNewClient)> m_cbNewClientCallback;
		std::function<void(SOCKET skt)> m_cbNewClientCallbackEx;
	};

	/**
	@brief [select]ģ������ͻ���.
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
		@brief ����������.
		\param nProType
			���紫��Э��.
		\param ip
			IP��ַ
		\param port
			�˿�
		\param bConn
			�Ƿ�����,���[nProType=TRANS_PROTOCOL_TYPE_UDP]�ɲ�����
		*/
		bool Open(UTIL_NET::TRANS_PROTOCOL_TYPE nProType, const char* ip, int port, bool bConn = true);

		/*
		@brief �����׽���.
			�ӿ��ڲ��Զ�����Ϊ [TRANS_PROTOCOL_TYPE_TCP]
		\param skt
			��Ч�׽���
		*/
		void Attach(SOCKET skt);

		/**
		@brief �Ƿ��ʹ���.
		\return
		*/
		bool IsError() const;
	protected:
		/**
		@brief ��ȡͨ��ʵ��.
		*/
		UTILS::NET::SELECT::CNet& GetObjectIns() { return m_Net; }

		/**
		@brief ���ݽ����߳�.
		\return
		*/
		void CheckThread(BOOL& bRun, CNetCmd* pBuffer);
	private:
		/**< ����ͨ��ʵ��.*/
		UTILS::NET::SELECT::CNetClient m_Net;
	};

	/*
	@brief �����׽��ֵı�������.
	\param skt
		��Ч�׽���
	\param ip
		���IP��ַ�Ļ�����
	\param len
		��������С
	\param port
		�˿�
	*/
	int UTIL_NET_CPPAPI GetSktName(SOCKET skt, char* ip, int len, int* port);
	
	/*
	@brief ��ȡ�ڵ��IP���˿�.
	\param addr
		�ڵ��ַ��Ϣ
	\param ip
		ip������
	param port
		port������
	*/
	int UTIL_NET_CPPAPI GetSktNameEx(sockaddr& addr, char* ip, int len, int* port);

	/*
	@brief ���[sockaddr]�ṹ��.
	\param ip
		���IP��ַ�Ļ�����
	\param port
		�˿�
	param addr
		���[sockaddr]�ṹ��.������
	*/
	int UTIL_NET_CPPAPI StuffSockAddr(TRANS_PROTOCOL_TYPE nType, char* ip, int port, sockaddr& addr);
}
#endif