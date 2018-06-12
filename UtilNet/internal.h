#ifndef __UTIL_NET_INTERNAL__
#define __UTIL_NET_INTERNAL__

/*
@brief ������
*/
enum NET_ERROR {
	NET_ERROR_SUCCESS = 0,
	NET_ERROR_PAR = -1,
	NET_ERROR_FAIL = -2,
	NET_ERROR_TIMEOUT = -3,
	NET_ERROR_LOCK = -4,
	NET_ERROR_BUSY = -5,
	NET_ERROR_INIT = -6,
	NET_ERROR_MALLOC = -7,
	NET_ERROR_FMT_NOT_STD = -8,//�Ǳ�׼
	NET_ERROR_OPERATOR_DB = -9,//�������ݿ�ʧ��.
	NET_ERROR_CONNECT = -10,//δ����ͨ��.
};


/*
@brief ����������Զ���������֤��Ϣ.
	��ֹ���ݵ�ַ�������,����������ڴ�
*/
typedef struct _INC_PATCH_NET_CONTEXT_PORPERTY
{
	/**< �ṹ��С.*/
	unsigned int uiSize;
	/**< �Զ������ݵ�ַ.Ҫ��Ψһ.*/
	unsigned int uiContext;
	/**< �Զ�����������.*/
	INC_PATCH_CONTEXT_DATA_TYPE nDataType;
	/**< �Զ�������.*/
	unsigned int uiPar;
	/**< �Ƿ���Ҫ�ͷ��ڴ�.*/
	BOOL bFreeFlag;
	/**< ʱ���.�ڴ����ʱ��.*/
	DWORD dwTimeStamp;
	/**< ��ʱ,�ͷ��ڴ�.*/
	DWORD dwTimeOut;
}INC_PATCH_NET_CONTEXT_PORPERTY, *PINC_PATCH_NET_CONTEXT_PORPERTY;

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
	/**< ������.*/
	int iError;
	/**< �������ݳ���.*/
	uint32_t uiDataLen;
	/**< ��������.*/
	BYTE buff[4];
}NET_DATA_HEAD, *PNET_DATA_HEAD;

class CNetDataWrapper final {
private:
	/**< �ڲ���������С.*/
	unsigned int _buff_len;
	/**< �ڲ�������.*/
	BYTE _buff[1024*64];
	/**< �ڲ�������ָ��. _buff_len<=1024*64,_ptr_buffָ��_buff,����ָ����ϵ�ַ.*/
	BYTE* _ptr_buff;
	/**< ������.*/
	int _err;
public:
	CNetDataWrapper() :_ptr_buff(nullptr), _buff_len(0), _err(0){
		_buff[0] = '\0';
		_ptr_buff = _buff;
		_buff_len = 1024 * 64;
	}
	/*
	@brief ��������
		���ʹ�ö��ϵ�ַ,�ͷ��ڴ�
	*/
	~CNetDataWrapper() {
		if ((_buff_len > 1024 * 64) && (_ptr_buff != nullptr)){
			delete[] _ptr_buff;
			_ptr_buff = nullptr;
		}
	}

	CNetDataWrapper(CNetDataWrapper const &) = delete;
	CNetDataWrapper(CNetDataWrapper &&) = delete;
	CNetDataWrapper& operator= (CNetDataWrapper &&) = delete;

	CNetDataWrapper& operator= (CNetDataWrapper const& other)
	{
		if (this == &other) {
			return *this;
		}
		if ((_buff_len > 1024 * 64) && (_ptr_buff != nullptr)){
			delete[] _ptr_buff;
			_ptr_buff = nullptr;
		}
		_buff_len = other._buff_len;
		if (_buff_len > 1024 * 64){
			_buff_len = PAD_SIZE(_buff_len);
			_ptr_buff = new BYTE[_buff_len];
			if (nullptr == _ptr_buff) {
				_buff_len = 0;
			}
		}
		else{
			_ptr_buff = _buff;
			_buff_len = 1024 * 64;
		}
		if (_ptr_buff == nullptr || _buff_len <= 0){
			_err = -1;
			return *this;
		}
		if (other._ptr_buff == nullptr){
			return *this;
		}

		PNET_DATA_HEAD info = (PNET_DATA_HEAD)other._ptr_buff;
		memcpy(_ptr_buff, other._ptr_buff, min(_buff_len, info->uiSize + info->uiDataLen));
		return *this;
	}

	/*
	@brief ��ʼ��
		һ����Ϊ���ݷ��ʹ���
		�ڲ���������С��[1024*64],����������������ֵ,���ʹ�� new���������·���.
	\param buff_len
		ָ����������С
	*/
	int Init(int buff_len = 0) {
		_err = 0;
		if (buff_len > 1024 * 64) {
			_buff_len = PAD_SIZE(buff_len);
			_ptr_buff = new BYTE[_buff_len];
			if (nullptr == _ptr_buff) {
				_buff_len = 0;
			}
		}
		else {
			_ptr_buff = _buff;
			_buff_len = 1024 * 64;
		}
		if (_ptr_buff == nullptr) {
			_err = -1;
			return _err;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		memset(info, 0, sizeof(NET_DATA_HEAD));
		info->uiSize = sizeof(NET_DATA_HEAD);
		info->uiNetFlag = NET_DATA_HEAD_FLAG;
		return _err;
	}

	/*
	@brief ��ʼ��
		һ����Ϊ���ݶ�ȡ����(��ʽ��),������������,����������//info->iSize + info->iDataLen
	\param p
		���ݻ�����. ������������[NET_DATA_HEAD]�ṹ���ݿ�ʼ.
	\param len
		���ݻ�������С.
	*/
	int Init(BYTE* p, int len) {
		_err = 0;
		if (nullptr == p) {
			_err = -1;
			return _err;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)p;
		if (info->uiSize != sizeof(NET_DATA_HEAD)) {
			_err = -2;
			return _err;
		}

		if (info->uiNetFlag != NET_DATA_HEAD_FLAG){
			_err = -3;
			return _err;
		}

		if (info->uiSize + info->uiDataLen != len) {
			_err = -4;
			return _err;
		}
		_ptr_buff = p;
		return _err;
	}
	/*
	@brief ��֤�����Ƿ�����,ִ���Ƿ�ɹ�.
	*/
	int Verity(){
		if (nullptr == _ptr_buff) {
			_err = -1;
			return _err;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		if (info->uiSize != sizeof(NET_DATA_HEAD)) {
			_err = -2;
			return _err;
		}

		if (info->uiNetFlag != NET_DATA_HEAD_FLAG){
			_err = -3;
			return _err;
		}

		if (info->iError != 0){
			_err = -4;
			return _err;
		}
		return _err;
	}
	unsigned int GetContext(bool self = true) {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return 0;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return self ? info->uiSelfContext : info->uiPeerContext;
	}
	void SetContext(unsigned int v, bool self = true) {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		unsigned int* pInt = nullptr;
		if (self) {
			pInt = &info->uiSelfContext;
		}
		else {
			pInt = &info->uiPeerContext;
		}
		*pInt = v;
	}
	int GetBuffLen() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return _err;
		}
		return _buff_len;
	}
	int GetDataBuffLen() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return _err;
		}
		return _buff_len - sizeof(NET_DATA_HEAD);
	}
	void SetNetMajorCmd(INC_PATCH_NET_CMD_TYPE n) {
		//assert(nullptr != _ptr_buff);
		if (nullptr == _ptr_buff) {
			_err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		info->uiMajorrCmd = n;
	}
	INC_PATCH_NET_CMD_TYPE GetNetMajorCmd() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return (INC_PATCH_NET_CMD_TYPE)-1;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return (INC_PATCH_NET_CMD_TYPE)info->uiMajorrCmd;
	}
	void SetNetMinorCmd(INC_PATCH_NET_MINOR_CMD_TYPE n) {
		//assert(nullptr != _ptr_buff);
		if (nullptr == _ptr_buff) {
			_err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		info->uiMinorCmd = n;
	}
	INC_PATCH_NET_MINOR_CMD_TYPE GetNetMinorCmd() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return (INC_PATCH_NET_MINOR_CMD_TYPE)-1;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return (INC_PATCH_NET_MINOR_CMD_TYPE)info->uiMinorCmd;
	}
	void SetDataLen(unsigned int n) {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		info->uiDataLen = n;
	}
	unsigned int GetDataLen() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return -1;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return info->uiDataLen;
	}
	unsigned int GetTotalLen() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return -1;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return info->uiSize + info->uiDataLen;
	}
	unsigned int GetHeadLen() {
		return sizeof(NET_DATA_HEAD);
	}
	BYTE* GetBuffPtr() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return NULL;
		}
		return _ptr_buff;
	}
	BYTE* GetDataPtr() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return NULL;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return info->buff;
	}
	void SetErrorCode(int err) {
		if (nullptr == _ptr_buff) {
			err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		info->iError = err;
	}
	int GetErrorCode() {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return -1;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return info->iError;
	}
	void SetFinish(BOOL v) {
		if (nullptr == _ptr_buff) {
			_err = -1;
			return;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		info->bFinish = v;
	}
	BOOL GetFinish() const {
		if (nullptr == _ptr_buff) {
			return FALSE;
		}
		PNET_DATA_HEAD info = (PNET_DATA_HEAD)_ptr_buff;
		return info->bFinish;
	}

	bool IsError() const {
		return _err != 0;
	}
	operator bool() {
		return !IsError();
	}
	bool operator==(const bool&rhs) {
		return IsError() == rhs;
	}
};
inline bool operator==(const CNetDataWrapper&lhs, bool rhs) {
	return lhs.IsError() == rhs;
}

inline bool operator==(const bool&rhs, const CNetDataWrapper&lhs) {
	return lhs.IsError() == rhs;
}


#endif