#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <Shlobj.h>
#include <Shlwapi.h>
using namespace std;

//����cookie������ͷ�ļ�
#include <wininet.h>
#include <UrlHist.h>
#pragma comment(lib,"Wininet.lib")

#pragma comment(lib,"Shlwapi.lib")
#define SWEEP_BUFFER_SIZE 100000
//������IDö��
enum DEL_RUBBISH_TYPE
{
	CLEAN_SYSTEM_TEMP = 0,     //ϵͳ��ʱ�ļ�
	CLEAN_RUBBISH_STATION,     //��������վ
	CLEAN_RUN_HISTORY,         //���м�¼
	CLEAN_DOCUMENT_HISTORY,    //����ĵ���¼
	CLEAN_PREVUSER_HISTORY,    //�û���¼��¼
	CLEAN_FILEFIND_HISTORY,    //�ļ����Ҽ�¼

	CLEAN_INTERNET_TEMP,       //Internet��ʱ�ļ�
	CLEAN_INTERNET_COOKIE,     //Internet Cookie
	CLEAN_ADDRBAR_HISTORY,     //��ַ����ʷ��¼
	CLEAN_PASSWORD_HISTORY,    //�����¼
	CLEAN_BROWSEADDR_HISTORY   //�����ַ��¼
};

//����Ļ�������
enum DEL_CACHE_TYPE
{
	FILES = 0, COOKIES
};

//�����ļ���Ϣ
typedef struct _RUBBISH_FILE_INFO
{
	WCHAR wzFilePath[MAX_PATH];
	WCHAR wzFileSize[0x10];
}RUBBISH_FILE_INFO, *PRUBBISH_FILE_INFO;

DWORD WINAPI CleanThread(LPVOID lpVoid);
class Rubbish
{
public:
	Rubbish();
	~Rubbish();
	BOOL StartCleanThread();

	VOID StartScan();

	VOID CleanRubbish(int CleanType);

	VOID CleanSystemTemp();

	BOOL EmptyDirectory(LPCTSTR szTempPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat=FALSE);

	BOOL ShowFileSize(DOUBLE dwSize, WCHAR * wzFileSize);

	BOOL WipeFile(LPCTSTR wzDirectory, LPCTSTR wzFilePath);

	VOID PrintRubbishInfo();

	VOID CleanRubbishStation();

	VOID CleanRunHistory();

	VOID CleanDocumentHistory();

	VOID CleanPrevUserHistory();

	VOID CleanFileFindHistory();

	VOID CleanInternetTemp();

	VOID CleanInternetCookie();

	VOID CleanAddressBarHistory();

	VOID CleanPasswordHistory();

	VOID CleanBrowseAddressHistory();

	BOOL DeleteUrlCache(DEL_CACHE_TYPE Type);

	
	BOOL m_IsScanFile;
private:
	vector<RUBBISH_FILE_INFO> m_RubbishFiles;
	double m_AllRubbishSize;
};