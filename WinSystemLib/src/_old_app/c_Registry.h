
class Registry;

#ifndef DEF_REGISTRY
#define DEF_REGISTRY

// -- -- -- -- 関連構造体 -- -- -- -- //

struct REG_ROOT_KEY{
	wchar root_name[256];
	wchar key_name[256];
	wchar *getRootKeyName();
};

struct REG_ROOT_KEY_VALUE : public REG_ROOT_KEY{
	wchar value_name[256];
};

struct REG_KEY_VALUE{
	wchar key_name[256];
	wchar value_name[256];
};

struct REG_DATA{
	DWORD mode;
	DWORD ndata;
	BYTE data[1];
	const wchar *toString();
};


// -- -- -- -- 関連関数 -- -- -- -- //

DWORD regGetModeWord(const wchar *name);
const wchar *regGetModeName(DWORD mode);
HKEY regGetRootHandle(const wchar *name);
const wchar *regGetRootName(HKEY handle);

bool regSplitRootKey(const wchar *key,REG_ROOT_KEY *reg);
bool regSplitRootKeyValue(const wchar *key,REG_ROOT_KEY_VALUE *reg);

REG_DATA *regConvertData(const wchar *source,DWORD mode);


// -- -- -- -- クラス本体 -- -- -- -- //

class Registry{
protected:
	HKEY hkeyParent;
	HKEY hkeyCurrent;
public:
	Registry();
	virtual ~Registry();
	bool exist(const wchar *key);
	bool vexistf(const wchar *key,va_list mark);
	bool exist(REG_ROOT_KEY *reg);
	bool open(bool create,const wchar *key);
	bool vopenf(bool create,const wchar *key,va_list mark);
	bool open(bool create,REG_ROOT_KEY *reg);
	bool openf(bool create,const wchar *key,...);
	bool existf(const wchar *key,...);
//	bool open(const wchar *key);
//	bool openf(const wchar *key,...);
//	bool open(REG_ROOT_KEY *reg);
	bool close();
	//
	bool set(wchar *name,REG_DATA *value);
	bool setText(wchar *name,wchar *text);

};






#endif


