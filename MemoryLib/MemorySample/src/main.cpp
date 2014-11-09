#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#include <assert.h>
#include <MemoryLib.h>

#ifndef _countof
#define _countof(A) (sizeof(A)/sizeof(A[0]))
#endif

//#####
extern bool g_debug;
extern int g_count;

class CTimeWatch{
public:
	CTimeWatch()
	{
		m_dwStart = 0;
	}
	void Start()
	{
		m_dwStart = timeGetTime();
	}
	void ShowResult(const char* szLabel)
	{
		DWORD dwTime = timeGetTime() - m_dwStart;
		printf("%hs: %d ms\n", szLabel, dwTime);
	}
private:
	DWORD m_dwStart;
};

int main()
{
	timeBeginPeriod(1);
	timeGetTime();
//	srand(timeGetTime());
	srand(4);

	// 正常性テスト２ //
	CNormalAllocator a;
	if(0){
		void* table[3];
		table[0] = a.Alloc(32);
		table[1] = a.Alloc(32);
		table[2] = a.Alloc(32);
		memset(table[0], 1, 32);
		memset(table[1], 2, 32);
		memset(table[2], 3, 32);
		a.Free(table[0]);
		a.Free(table[1]);
		a.Free(table[2]);

		return 0;
	}
	// パフォーマンス測定 //
	if(1){
		CTimeWatch t;
		int nCount = 100*10000; //100万回
		void* table[100] = {0};

		for(int round = 0; round < 5; round++){
			// malloc
			memset(table, 0, sizeof(table));
			t.Start();
			for(int i = 0; i < nCount; i++){
				int size = rand() % 3000;
				int index = rand() % 100;
				if(!table[index]){
					table[index] = ::malloc(size);
				}
				else{
					::free(table[index]);
					table[index] = NULL;
				}
			}
			t.ShowResult("malloc   ");

			// select allocator
			memset(table, 0, sizeof(table));
			t.Start();
			for(int i = 0; i < nCount; i++){
				int size = rand() % 3000;
				int index = rand() % 100;
				if(!table[index]){
					table[index] = CSelectAllocator::Alloc(size);
				}
				else{
					CSelectAllocator::Free(table[index]);
					table[index] = NULL;
				}
			}
			t.ShowResult("allocator");
		}
		return 0;
	}

	// パフォーマンス測定 //
	if(1){
		CTimeWatch t;
		int nCount = 100*10000; //100万回
		void* table[100] = {0};

		for(int round = 0; round < 5; round++){
			// malloc
			memset(table, 0, sizeof(table));
			t.Start();
			for(int i = 0; i < 0*nCount; i++){
				int size = rand() % 1000;
				int index = rand() % 100;
				if(!table[index]){
					table[index] = ::malloc(size);
				}
				else{
					::free(table[index]);
					table[index] = NULL;
				}
			}
			t.ShowResult("malloc   ");

			// select allocator
			memset(table, 0, sizeof(table));
			t.Start();
			for(int i = 0; i < nCount; i++){
				int size = rand() % 100;
				int index = rand() % 100;

				if(round==0)continue;

				//###
				if(g_debug){
					printf("%d\n", i);
				}
				if(!table[index]){
					table[index] = a.Alloc(size);
					//###
					if(g_debug){
//						a.Free(table[index]);
//						table[index] = NULL;
					}
				}
				else{
					a.Free(table[index]);
					table[index] = NULL;
				}
			}
			t.ShowResult("allocator");
		}
	}

	// 正常性テスト //
	if(0){
		std::vector<void*> v;
		for(int i = 0; i < 10; i++){
			void* p = CSelectAllocator::Alloc(10);
			printf("p=%p\n", p);
			CSelectAllocator::Free(p);
		}

		printf("----\n");
		for(int i = 0; i < 10; i++){
			void* p = CSelectAllocator::Alloc(10);
			printf("p=%p\n", p);
			v.push_back(p);
		}
		for(int i = 0; i < 10; i++){
			CSelectAllocator::Free(v[i]);
		}
		v.clear();

		printf("----\n");
		for(int i = 0; i < 10; i++){
			void* p = CSelectAllocator::Alloc(10);
			printf("p=%p\n", p);
			CSelectAllocator::Free(p);
		}
	}
	
	/*
	void* p = malloc(1);
	int* i = new int;
	int* a = new int[10];
	free(p);
	delete i;
	delete[] a;
	*/
	return 0;
}
