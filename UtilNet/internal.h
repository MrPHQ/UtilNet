#ifndef __UTIL_NET_INTERNAL__
#define __UTIL_NET_INTERNAL__

/*
@brief 错误码
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
	NET_ERROR_FMT_NOT_STD = -8,//非标准
	NET_ERROR_OPERATOR_DB = -9,//操作数据库失败.
	NET_ERROR_CONNECT = -10,//未建立通信.
};


/*
@brief 网络命令传输自定义数据验证信息.
	防止数据地址传输错误,操作错误的内存
*/
typedef struct _INC_PATCH_NET_CONTEXT_PORPERTY
{
	/**< 结构大小.*/
	unsigned int uiSize;
	/**< 自定义数据地址.要求唯一.*/
	unsigned int uiContext;
	/**< 自定义数据类型.*/
	INC_PATCH_CONTEXT_DATA_TYPE nDataType;
	/**< 自定义数据.*/
	unsigned int uiPar;
	/**< 是否需要释放内存.*/
	BOOL bFreeFlag;
	/**< 时间戳.内存分配时间.*/
	DWORD dwTimeStamp;
	/**< 超时,释放内存.*/
	DWORD dwTimeOut;
}INC_PATCH_NET_CONTEXT_PORPERTY, *PINC_PATCH_NET_CONTEXT_PORPERTY;

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
	/**< 错误码.*/
	int iError;
	/**< 附带数据长度.*/
	uint32_t uiDataLen;
	/**< 附带数据.*/
	BYTE buff[4];
}NET_DATA_HEAD, *PNET_DATA_HEAD;

class CNetDataWrapper final {
private:
	/**< 内部缓存区大小.*/
	unsigned int _buff_len;
	/**< 内部缓存区.*/
	BYTE _buff[1024*64];
	/**< 内部缓存区指针. _buff_len<=1024*64,_ptr_buff指向_buff,否则指向堆上地址.*/
	BYTE* _ptr_buff;
	/**< 错误码.*/
	int _err;
public:
	CNetDataWrapper() :_ptr_buff(nullptr), _buff_len(0), _err(0){
		_buff[0] = '\0';
		_ptr_buff = _buff;
		_buff_len = 1024 * 64;
	}
	/*
	@brief 析构函数
		如果使用堆上地址,释放内存
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
	@brief 初始化
		一般作为数据发送处理
		内部缓存区大小是[1024*64],如果缓存区大于这个值,则会使用 new操作符重新分配.
	\param buff_len
		指定缓存区大小
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
	@brief 初始化
		一般作为数据读取处理(格式化),必须整包处理,即满足条件//info->iSize + info->iDataLen
	\param p
		数据缓存区. 缓存区数据以[NET_DATA_HEAD]结构数据开始.
	\param len
		数据缓存区大小.
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
	@brief 验证数据是否完整,执行是否成功.
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