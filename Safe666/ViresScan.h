#pragma once
#include <Windows.h>
#include <vector>

#include "MD5.h"

class CViresScan
{
public:
	CViresScan();
	~CViresScan();
	void CheckSelect();
	void StartScan();
	void ScanSingle(WCHAR * wzPath);
	void ScanAll(WCHAR * wzPath);
	BOOL MD5Scan(WCHAR * wzPath);
	BOOL WhiteScan(WCHAR * wzPath);
private:
	
	WCHAR m_Path[MAX_PATH];  //ɨ��Ϊ�����ļ�����ȫ·��
	WCHAR m_Way[MAX_PATH];   //ɨ��ΪMD5���߰�����
	WCHAR m_SettingPath[MAX_PATH]; //�����ļ�·��
	WCHAR m_ScanPath[MAX_PATH];    //��ɱ·��
	WCHAR m_MD5[32];          //��¼MD5��ֵ
	std::vector<WCHAR*> m_LocalMD5;		//����MD5��
};

