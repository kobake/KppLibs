#pragma once

#include "../ThreadLib/src/CCriticalSection.h"

template <class T> class TSingleton{
public:
	//�A�N�Z�T
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
	//�폜
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

	//���z�f�X�g���N�^
	virtual ~TSingleton()
	{
		if(gm_pInstance == this){
			gm_pInstance = NULL;
		}
	}
private:
	//�C���X�^���X
	static T*				gm_pInstance;
	static CCriticalSection	gm_cCricitalSection;
};
template <class T> T*				TSingleton<T>::gm_pInstance=NULL;
template <class T> CCriticalSection	TSingleton<T>::gm_cCricitalSection;
