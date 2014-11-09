#pragma once

#include <windows.h> //LocalAlloc

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
アロケータ実装用シングルトン。
デフォルトの operator new/operator delete を呼ばないようにしている。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
template <class T> class TSystemSingleton{
public:
	// メモリ //
	void*	operator new(size_t size)
	{
		return ::LocalAlloc(LMEM_FIXED, size);
	}
	void	operator delete(void* p)
	{
		::LocalFree((HLOCAL)p);
	}
public:
	// アクセサ //
	static T* Instance()
	{
		if(!gm_pInstance){
			atexit(DisposeSingleton);
			gm_pInstance=new T();
		}
		return gm_pInstance;
	}
public:
	// 削除 //
	static void DisposeSingleton()
	{
		delete gm_pInstance;
		gm_pInstance=NULL;
	}
public:
	// 仮想デストラクタ //
	virtual ~TSystemSingleton()
	{
	}
private:
	//インスタンス
	static T*				gm_pInstance;
};
template <class T> T*		TSystemSingleton<T>::gm_pInstance=NULL;

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
アロケータ。
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
class CNormalAllocator : public TSystemSingleton<CNormalAllocator>{
public:
	void*	Alloc(size_t size);
	void	Free(void* p);
};
