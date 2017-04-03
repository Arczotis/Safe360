#include "stdafx.h"
#include "ViresScan.h"


CViresScan::CViresScan()
{
	CheckSelect();
}


CViresScan::~CViresScan()
{
}

void CViresScan::CheckSelect()
{
	
	GetCurrentDirectory(MAX_PATH, m_SettingPath);
	lstrcatW(m_SettingPath, L"\\setting.ini");
	//��������ļ���ѡ���ɨ�跽ʽ
	GetPrivateProfileString(L"ScanWay", L"Path", NULL, m_Path, MAX_PATH, m_SettingPath);
	//��������ļ���ѡ��Ĳ�ɱ��ʽ
	GetPrivateProfileString(L"ScanWay", L"Way", NULL, m_Way, MAX_PATH, m_SettingPath);

	//��ȡ����MD5��
	for (int i = 0; i < 3; i++)
	{
		WCHAR Index[10];
		wsprintf(Index, L"%d", i);
		WCHAR v1[32+1] = {0};
		GetPrivateProfileString(L"MD5", Index, NULL, v1, MAX_PATH, m_SettingPath);
		m_LocalMD5.push_back(v1);
	}
}

void CViresScan::StartScan()
{
	//���ļ�ɨ��
	if (lstrcmp(L"0", m_Path)==0)
	{
		//��������ļ���ѡ��Ĳ�ɱ·��
		GetPrivateProfileString(L"ScanPath", L"SinglePath", NULL,m_ScanPath,MAX_PATH,m_SettingPath);
		
		ScanSingle(m_ScanPath);
	}
	else
	{
		//��������ļ���ѡ��Ĳ�ɱ·��
		GetPrivateProfileString(L"ScanPath", L"Directory", NULL, m_ScanPath, MAX_PATH, m_SettingPath);
		ScanAll(m_ScanPath);
	}

}

//���ļ�ɨ��
void CViresScan::ScanSingle(WCHAR* wzPath)
{
	WIN32_FIND_DATA Win32FindData;

	DWORD dwAttributes = 0;

	FindFirstFile(wzPath, &Win32FindData);

	//�ж��Ƿ���MD5��ɱ
	if (lstrcmp(L"0", m_Way) == 0)
	{
		//MD5��ɱ
		if (MD5Scan(wzPath))
		{
			printf("���ļ��ǲ���!MD5:%S",m_MD5);
			printf(" \r\n");
		}
		else
		{
			printf("���ļ����ǲ���!MD5:%S", m_MD5);
			printf(" \r\n");
		}
	}
	else
	{
		//��������ɱ
		if (WhiteScan(wzPath))
		{
			
		}
	}
}

//ȫ·��ɨ��
void CViresScan::ScanAll(WCHAR* wzPath)
{

}

//MD5
BOOL CViresScan::MD5Scan(WCHAR* wzPath)
{
	//LPCTSTR תCHAR*
	int Length = WideCharToMultiByte(CP_OEMCP, NULL, wzPath, -1, NULL, 0, NULL, FALSE);
	char *v1 = new char[Length];
	WideCharToMultiByte(CP_OEMCP, NULL, wzPath, -1, v1, Length, NULL, FALSE);
	//��ȡMD5ֵ
	char* MD5 = MD5FileValue(v1);
	//CHAR* תLPCTSTR
	Length = MultiByteToWideChar(0, 0, MD5, -1, NULL, 0);
	WCHAR *v2 = new WCHAR[Length];
	MultiByteToWideChar(0, 0, MD5, -1, v2, Length);
	memcpy(m_MD5, v2, Length);
	delete[]v1;
	delete[]v2;

	//��ȡ�ļ�MD5��Ϣ���
	//����MD5��ɱ		��m_LocalMD5���Ա�
	for (DWORD i = 0; i < m_LocalMD5.size(); i++)
	{
		if (*m_LocalMD5[i] == *m_MD5)
		{
			//�ǲ��� ������
			return TRUE;
		}
	}
	return FALSE;
}

//������
BOOL CViresScan::WhiteScan(WCHAR* wzPath)
{

	return TRUE;
}