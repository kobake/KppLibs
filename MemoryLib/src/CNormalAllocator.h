#pragma once

#include <windows.h> //LocalAlloc

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�A���P�[�^�����p�V���O���g���B
�f�t�H���g�� operator new/operator delete ���Ă΂Ȃ��悤�ɂ��Ă���B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
template <class T> class TSystemSingleton{
public:
	// ������ //
	void*	operator new(size_t size)
	{
		return ::LocalAlloc(LMEM_FIXED, size);
	}
	void	operator delete(void* p)
	{
		::LocalFree((HLOCAL)p);
	}
public:
	// �A�N�Z�T //
	static T* Instance()
	{
		if(!gm_pInstance){
			atexit(DisposeSingleton);
			gm_pInstance=new T();
		}
		return gm_pInstance;
	}
public:
	// �폜 //
	static void DisposeSingleton()
	{
		delete gm_pInstance;
		gm_pInstance=NULL;
	}
public:
	// ���z�f�X�g���N�^ //
	virtual ~TSystemSingleton()
	{
	}
private:
	//�C���X�^���X
	static T*				gm_pInstance;
};
template <class T> T*		TSystemSingleton<T>::gm_pInstance=NULL;

/* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
�A���P�[�^�B
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
class CNormalAllocator : public TSystemSingleton<CNormalAllocator>{
public:
	void*	Alloc(size_t size);
	void	Free(void* p);
};
