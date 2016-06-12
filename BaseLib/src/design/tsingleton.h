#pragma once

#include "../ThreadLib/src/CCriticalSection.h"

template <class T> class TSingleton{
public:
	//アクセサ
	static T* Instance()
	{
		CRITICAL_ENTER(gm_cCricitalSection);

		if(!gm_pInstance){
			atexit(DisposeSingleton);
			gm_pInstance=new T();
		}
		return gm_pInstance;
	}
public:
	//削除
	static void DisposeSingleton()
	{
		CRITICAL_ENTER(gm_cCricitalSection);

		delete gm_pInstance;
		gm_pInstance=NULL;
	}
public:
	TSingleton()
	{
		gm_pInstance = static_cast<T*>(this);
	}

	//仮想デストラクタ
	virtual ~TSingleton()
	{
		if(gm_pInstance == this){
			gm_pInstance = NULL;
		}
	}
private:
	//インスタンス
	static T*				gm_pInstance;
	static CCriticalSection	gm_cCricitalSection;
};
template <class T> T*				TSingleton<T>::gm_pInstance=NULL;
template <class T> CCriticalSection	TSingleton<T>::gm_cCricitalSection;
