#pragma once
#include<Windows.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

#define Win32PATH _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define Win64PATH _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")

//�����Ϣ�ṹ��
typedef struct _SOFTWAREINFO
{
	TCHAR wzSoftName[50];						//�������
	TCHAR wzSoftVersion[50];					//����汾��
	TCHAR wzSoftPublisher[50];					//�����������
	TCHAR wzSoftDate[20];						//�����װ����
	TCHAR wzSoftInstallPath[MAX_PATH];			//�����װ·��
	TCHAR wzSoftUninstallPath[MAX_PATH];		//���ж��·��
}SOFTWAREINFO, *PSOFTWAREINFO;

class SoftwareManager
{
public:
	SoftwareManager();
	~SoftwareManager();

	void GetSoftwareInfo();
	BOOL IsX64();
	BOOL PrintSoftwareInfo();
private:
	vector<SOFTWAREINFO> m_SoftwareInfo;
};

