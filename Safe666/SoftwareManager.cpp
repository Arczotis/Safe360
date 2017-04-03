#include "stdafx.h"
#include "SoftwareManager.h"


SoftwareManager::SoftwareManager()
{
}


SoftwareManager::~SoftwareManager()
{
}

//ͨ����ȡע����ȡ�����Ϣ
void SoftwareManager::GetSoftwareInfo()
{
	SOFTWAREINFO SoftwareInfo = {0};
	HKEY hkRootKey;          // ����
	LPCTSTR lpSubKey[2];        // �Ӽ�����
	HKEY hkResult;           // ��Ҫ�򿪼��ľ�� 
	HKEY hkRKey;
	LONG Return;             // ��¼��ȡע����Ƿ�ɹ�
	TCHAR wzKeyName[255] = { 0 };        // ע���������
	
	hkRootKey = HKEY_LOCAL_MACHINE;
	//32λEXE
	lpSubKey[0] = Win32PATH;
	//64λEXE
	lpSubKey[1] = Win64PATH;
	printf("------------------�����Ϣ-----------------\r\n");
	for (int i = 0; i < 2; i++)
	{
		//��ע���
		Return = RegOpenKeyEx(hkRootKey, lpSubKey[i], 0,
			KEY_ENUMERATE_SUB_KEYS, &hkResult);

		WCHAR wzBuffer[255] = { 0 };

		DWORD dwIndex = 0;
		if (Return == ERROR_SUCCESS)
		{

			DWORD dwListIndex = 0;
			DWORD dwKeyLength = 255;
			
			//Ҫ��ʹ��RegEnumKeyEx��������KEY_ENUMERATE_SUB_KEYSȨ�޲���
			while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hkResult, dwIndex, wzKeyName, &dwKeyLength, NULL, NULL, NULL, NULL))
			{
				if (wzKeyName != L"")
				{
					lstrcat(wzBuffer, (LPWSTR)lpSubKey);
					lstrcat(wzBuffer, L"\\");
					lstrcat(wzBuffer, wzKeyName);
					DWORD dwType = 0;
					DWORD dwLength = 255;
					if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, wzBuffer,
						NULL, KEY_QUERY_VALUE, &hkRKey) == ERROR_SUCCESS)
					{

						DWORD Ret = RegQueryValueEx(hkRKey, _T("DisplayName"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftName, &dwLength);
						dwLength = 255;

						RegQueryValueEx(hkRKey, _T("DisplayVersion"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftVersion, &dwLength);
						dwLength = 255;

						RegQueryValueEx(hkRKey, _T("InstallLocation"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftInstallPath, &dwLength);
						dwLength = 255;

						RegQueryValueEx(hkRKey, _T("Publisher"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftPublisher, &dwLength);
						dwLength = 255;

						RegQueryValueEx(hkRKey, _T("UninstallString"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftUninstallPath, &dwLength);
						dwLength = 255;

						RegQueryValueEx(hkRKey, _T("InstallDate"),
							NULL, &dwType, (LPBYTE)SoftwareInfo.wzSoftDate, &dwLength);
						if (SoftwareInfo.wzSoftDate[0])
						{
							SoftwareInfo.wzSoftDate[9] = SoftwareInfo.wzSoftDate[7];
							SoftwareInfo.wzSoftDate[8] = SoftwareInfo.wzSoftDate[6];
							SoftwareInfo.wzSoftDate[7] = '-';
							SoftwareInfo.wzSoftDate[6] = SoftwareInfo.wzSoftDate[5];
							SoftwareInfo.wzSoftDate[5] = SoftwareInfo.wzSoftDate[4];
							SoftwareInfo.wzSoftDate[4] = '-';
						}
						dwLength = 255;

						//��������
						if (SoftwareInfo.wzSoftName[0] != '\0')
						{
							m_SoftwareInfo.push_back(SoftwareInfo);

						}

						dwIndex++;
					}
					dwKeyLength = 255;
					memset(wzKeyName, 0, 255);
					memset(wzBuffer, 0, 255);
					memset(&SoftwareInfo, 0, sizeof(SOFTWAREINFO));
				}
			}
			RegCloseKey(hkResult);
			
		}
	}
	printf("-------------------------------------------\r\n");
}

BOOL SoftwareManager::IsX64()
{
	SYSTEM_INFO SystemInfo;
	GetNativeSystemInfo(&SystemInfo);
	if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		SystemInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
	{
		//64 λ����ϵͳ
		return TRUE;
	}
	else
	{
		// 32 λ����ϵͳ
		return FALSE;
	}
	return FALSE;
}

BOOL SoftwareManager::PrintSoftwareInfo()
{
	vector<SOFTWAREINFO>::iterator SoftwareInfo;
	for (SoftwareInfo=m_SoftwareInfo.begin();SoftwareInfo!=m_SoftwareInfo.end();++SoftwareInfo)
	{
		printf("�������:%S \r\n", SoftwareInfo->wzSoftName);
		printf("����汾��:%S \r\n", SoftwareInfo->wzSoftVersion);
		printf("�����������:%S \r\n", SoftwareInfo->wzSoftPublisher);
		printf("�����װ����:%S \r\n", SoftwareInfo->wzSoftDate);
		printf("�����װ·��:%S \r\n", SoftwareInfo->wzSoftInstallPath);
		printf("���ж��·��:%S \r\n", SoftwareInfo->wzSoftUninstallPath);
	}
	return TRUE;
}
