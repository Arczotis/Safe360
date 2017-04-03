#include "stdafx.h"
#include "SelfRunning.h"


SelfRunning::SelfRunning()
{
}


SelfRunning::~SelfRunning()
{
}

void SelfRunning::GetSelfRunningSoftware()
{
	SELFRUNNINGINFO SelfRunningInfo = { 0 };

	HKEY hkRootKey = NULL;						// ����
	HKEY hkResult  = NULL;						// ��Ҫ�򿪼��ľ�� 
	LONG Return;						// ��¼��ȡע����Ƿ�ɹ�
	DWORD Index = 0;
	TCHAR szKeyName[255] = { 0 };       // ע���������
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;

	LPCTSTR lpSubKey[2];				// �Ӽ�����
	DWORD ListIndex = 0;

	lpSubKey[0] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	lpSubKey[1] = _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");
	
	for (DWORD j = 0; j < 2; j++)
	{
		if (j == 0)
			hkRootKey = HKEY_CURRENT_USER;
		if (j == 1)
			hkRootKey = HKEY_LOCAL_MACHINE;
		for (DWORD i = 0; i < 2; i++)
		{
			//��lpSubkey
			Return = RegOpenKeyEx(hkRootKey, lpSubKey[i], 0,
				KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, &hkResult);
			if (Return == ERROR_SUCCESS)
			{
				//����򿪳ɹ�����ʼö����ֵ
				while (ERROR_NO_MORE_ITEMS !=
					RegEnumValue(hkResult, Index, SelfRunningInfo.wzBootName, &dwKeyLen,
						0, NULL, NULL, NULL))
				{
					//��ȡÿ��ֵ������ szKeyNameΪֵ������ bufferΪֵ������
					RegQueryValueEx(hkResult, SelfRunningInfo.wzBootName,
						0, &dwType, (LPBYTE)SelfRunningInfo.wzBootOrder, &dwNameLen);

					//������Ϣ
					SelfRunningInfo.wzHKEY;
					if (j == 0)
						_tcscpy_s(SelfRunningInfo.wzHKEY, _T("HKEY_CURRENT_USER"));
					if (j == 1)
						_tcscpy_s(SelfRunningInfo.wzHKEY, _T("HKEY_LOCAL_MACHINE"));
					//ɾ��ʱҪ�õ���
					_tcscpy_s(SelfRunningInfo.wzBootPos, lpSubKey[i]);

					//��ӵ�vector
					m_SelfRunningSoftware.push_back(SelfRunningInfo);
					
					Index++;
					dwNameLen = 255;
					dwKeyLen = 255;
					memset(&SelfRunningInfo, 0, sizeof(SELFRUNNINGINFO));
				}
				Index = 0;	
			}		
		}
		//�ر�ע���
		RegCloseKey(hkResult);
	}
	
}


void SelfRunning::PrintInfo()
{
	printf("------------------���������----------------------\r\n");
	vector<SELFRUNNINGINFO>::iterator SelfRunningSoftware;
	for (SelfRunningSoftware = m_SelfRunningSoftware.begin(); SelfRunningSoftware != m_SelfRunningSoftware.end(); ++SelfRunningSoftware)
	{
		printf("************************************************\r\n");
		printf("�����%S\r\n", SelfRunningSoftware->wzBootName);
		printf("�������%S\r\n", SelfRunningSoftware->wzBootOrder);
		printf("ע�����Ϣ��%S\\%S\r\n", SelfRunningSoftware->wzHKEY, SelfRunningSoftware->wzBootPos);
		printf("************************************************\r\n");
	}
	printf("-------------------------------------------------\r\n");
}
