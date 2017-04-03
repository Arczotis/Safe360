#include "stdafx.h"
#include "Services.h"


Services::Services()
{
}


Services::~Services()
{
}


void Services::GetServicesInfo()
{
	//1.��Զ�̼����������ƹ�����
	SC_HANDLE SCHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	//2.��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		SCHandle,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		NULL,				//������
		0,					//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL,
		NULL);
	//3.������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS lpEnumService =
		(LPENUM_SERVICE_STATUS_PROCESS)LocalAlloc(LPTR, dwSize);
	//4.�ڶ���ö��
	EnumServicesStatusEx(
		SCHandle,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		(LPBYTE)lpEnumService,		//������
		dwSize,				//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL,
		NULL);
	//5.������Ϣ
	SC_HANDLE ServiceHandle = nullptr;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//��ȡ������Ϣ
		//1.��������
		//2.��������
		//3.����״̬(���ݵõ���ֵ�ֶ�����ַ���)
		//����ֹͣ�� ����������" "������ͣ"...
		SERVICE_STATUS_C ServiceStatus = {0};
		//��ȡ������Ϣ
		//1.�򿪷���
		ServiceHandle = OpenService(SCHandle,
			lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(ServiceHandle, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.�ڶ��ε���,��ȡ��Ϣ
		QueryServiceConfig(ServiceHandle, pServiceConfig, dwSize, &dwSize);
		//����list�и÷����״̬��Ϣ
		switch (lpEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_STOPPED:
			lstrcpy(ServiceStatus.wzCurrentState, L"��ֹͣ");
			break;
		case SERVICE_STOP_PENDING:
			lstrcpy(ServiceStatus.wzCurrentState, L"����ֹͣ");
			break;
		case SERVICE_START_PENDING:
			lstrcpy(ServiceStatus.wzCurrentState, L"��������");
			break;
		case SERVICE_RUNNING:
			lstrcpy(ServiceStatus.wzCurrentState, L"��������");
			break;
		case SERVICE_PAUSED:
			lstrcpy(ServiceStatus.wzCurrentState, L"����ͣ");
			break;
		case SERVICE_PAUSE_PENDING:
			lstrcpy(ServiceStatus.wzCurrentState, L"������ͣ");
			break;
		case SERVICE_CONTINUE_PENDING:
			lstrcpy(ServiceStatus.wzCurrentState, L"׼������");
			break;
		}

		switch (pServiceConfig->dwStartType)
		{
		case SERVICE_AUTO_START:
			lstrcpy(ServiceStatus.wzStartType, L"�Զ�");
			break;
		case SERVICE_BOOT_START:
			lstrcpy(ServiceStatus.wzStartType, L"SERVICE_BOOT_START");
			break;
		case SERVICE_DEMAND_START:
			lstrcpy(ServiceStatus.wzStartType, L"�ֶ�");
			break;
		case SERVICE_DISABLED:
			lstrcpy(ServiceStatus.wzStartType, L"����");
			break;
		case SERVICE_SYSTEM_START:
			lstrcpy(ServiceStatus.wzStartType, L"SERVICE_SYSTEM_START");
			break;
		}
		switch (lpEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			lstrcpy(ServiceStatus.wzServiceType, L"�ļ�ϵͳ��������");
			break;
		case SERVICE_KERNEL_DRIVER:
			lstrcpy(ServiceStatus.wzServiceType, L"��������");
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			lstrcpy(ServiceStatus.wzServiceType, L"�������̷���");
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			lstrcpy(ServiceStatus.wzServiceType, L"������̷���");
			break;
		}
		ServiceStatus.lpServiceName  = lpEnumService[i].lpServiceName;
		ServiceStatus.lpDisplayName  = lpEnumService[i].lpDisplayName;
		m_ServicesInfo.push_back(ServiceStatus);
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(SCHandle);
	CloseServiceHandle(ServiceHandle);
}

void Services::PrintServicesInfo()
{
	printf("----------------------ϵͳ����----------------------\r\n");
	vector<SERVICE_STATUS_C>::iterator ServiceInfo;
	for (ServiceInfo = m_ServicesInfo.begin(); ServiceInfo != m_ServicesInfo.end(); ++ServiceInfo)
	{
		//��ӡ�����ַ����������仰
		setlocale(LC_ALL, "");
		printf("********************************\r\n");
		printf("���ƣ�%S\r\n", ServiceInfo->lpServiceName);
		printf("������%S\r\n", ServiceInfo->lpDisplayName);
		printf("״̬��%S\r\n", ServiceInfo->wzCurrentState);
		printf("�������ͣ�%S\r\n", ServiceInfo->wzStartType);
		printf("�������ͣ�%S\r\n", ServiceInfo->wzServiceType);
		printf("********************************\r\n");
	}
	printf("---------------------------------------------------\r\n");
}