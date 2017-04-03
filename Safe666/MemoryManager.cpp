#include "stdafx.h"
#include "MemoryManager.h"


MemoryManager::MemoryManager()
{
	m_MemoryStatus = { sizeof(MEMORYSTATUSEX) };
}


MemoryManager::~MemoryManager()
{
}

BOOL MemoryManager::GetCPUStatus()
{
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc_CPU, this, 0, NULL);
	if (hThread)
	{
		return TRUE;
	}
	return FALSE;
}

DWORD WINAPI ThreadProc_CPU(LPVOID lpParam)
{
	HANDLE hEvent;
	FILETIME preidleTime, prekernelTime, preuserTime;
	FILETIME idleTime, kernelTime, userTime;
	DOUBLE FreeTime1;
	DOUBLE FreeTime2;
	DOUBLE KernelTime1;
	DOUBLE KernelTime2;
	DOUBLE UserTime1;
	DOUBLE UserTime2;
	DWORD  CPU;

	//1.��һ�λ�ȡ������ʱ��
	//����ϵͳʱ����Ϣ���ڶദ����ϵͳ�ϣ����ص�ֵ�����д�����֮��ָ��ʱ����ܺ͡�
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	//2.�����ں˶��󣬲��ҵȴ�1000����
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (1)
	{
		WaitForSingleObject(hEvent, 1000);
		//3.�ڶ��λ�ȡ������ʱ��
		GetSystemTimes(&idleTime, &kernelTime, &userTime);
		//4.ת��ʱ��
		FreeTime1 = (DOUBLE)(preidleTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)preidleTime.dwLowDateTime;
		FreeTime2 = (DOUBLE)(idleTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)idleTime.dwLowDateTime;
		KernelTime1 = (DOUBLE)(prekernelTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)prekernelTime.dwLowDateTime;
		KernelTime2 = (DOUBLE)(kernelTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)kernelTime.dwLowDateTime;
		UserTime1 = (DOUBLE)(preuserTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)preuserTime.dwLowDateTime;
		UserTime2 = (DOUBLE)(userTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)userTime.dwLowDateTime;
		//5.����CPUʹ����
		CPU = (DWORD)(100.0 - (FreeTime2 - FreeTime1) /
			(KernelTime2 - KernelTime1 + UserTime2 - UserTime1)*100.0);
		//6.�������ڷ�����Ϣ������CPUʹ����
		printf("CPUʹ����:%d%%\r\n",CPU);
		//7.���¸��ƣ�ѭ��ˢ��CPUʹ����
		preidleTime   = idleTime;
		prekernelTime = kernelTime;
		preuserTime   = userTime;
	}
	return 0;
}

BOOL MemoryManager::GetMemoryStatus()
{
	
	if (!GlobalMemoryStatusEx(&m_MemoryStatus))
	{
		printf("Cannot Get Memory Information!\r\n");
		return FALSE;
	}
	printf("-------------�ڴ���Ϣ-----------\r\n");
	printf("�����ڴ�������%f GB\r\n",(float)(m_MemoryStatus.ullTotalPhys/1024.0f/1024.0f/1024.0f));
	printf("���������ڴ棺%f GB\r\n", (float)(m_MemoryStatus.ullAvailPhys / 1024.0f / 1024.0f / 1024.0f));
	printf("��ʹ�ðٷֱȣ�%d%% \r\n", m_MemoryStatus.dwMemoryLoad);
	printf("��ҳ�ļ�������%f GB\r\n", (float)(m_MemoryStatus.ullTotalPageFile / 1024.0f / 1024.0f / 1024.0f));
	printf("���з�ҳ�ļ���%f GB\r\n", (float)(m_MemoryStatus.ullAvailPageFile / 1024.0f / 1024.0f / 1024.0f));
	printf("�����ڴ�������%f GB\r\n", (float)(m_MemoryStatus.ullTotalVirtual / 1024.0f / 1024.0f / 1024.0f));
	printf("���������ڴ棺%f GB\r\n", (float)(m_MemoryStatus.ullAvailVirtual / 1024.0f / 1024.0f / 1024.0f));
	printf("--------------------------------\r\n");
	return TRUE;
}

void MemoryManager::ClearMemory()
{
	printf("���������ڴ�...\r\n");

	HANDLE ProcessSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ProcessSnapHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);

	//��ȡϵͳ�е�һ�����̵���Ϣ  
	BOOL Status = Process32First(ProcessSnapHandle, &ProcessInfo);
	while (Status)
	{
		HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (ProcessHandle)
		{
			//���ý��̹�������С
			SetProcessWorkingSetSize(ProcessHandle, -1, -1);
			//�����ܶ�Ľ�ָ�����̵�ҳ��ӹ������Ƴ�
			EmptyWorkingSet(ProcessHandle);
			CloseHandle(ProcessHandle);
		}
		//��ȡ��һ�����̵���Ϣ  
		Status = Process32Next(ProcessSnapHandle, &ProcessInfo);
	}
}