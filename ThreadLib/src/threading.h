#pragma once

//�X���b�h
#include "CThread.h"

//����
#include "CEvent.h"
#include "CCriticalSection.h"
#include "CMutex.h"

//�L���[
#include "CThreadQueue.h"

//���
#include "CModule.h"
#include "CProcess.h"
#include "CProcessWatcher.h"
#include "CThreadInfo.h"

//�c�[��
#include "CSharedMemory.h"

//���C�u����
#pragma comment(lib,"psapi.lib") //GetModuleBaseName, GetModuleFileNameEx ��
