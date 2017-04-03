#include "stdafx.h"
#include "Rubbish.h"


Rubbish::Rubbish()
{
	m_IsScanFile = TRUE;
	m_AllRubbishSize = (double)0;
}


Rubbish::~Rubbish()
{
}

BOOL Rubbish::StartCleanThread()
{
	CloseHandle(CreateThread(NULL, 0, CleanThread, this, 0, NULL));
	return TRUE;
}

VOID Rubbish::StartScan()
{
	m_IsScanFile = TRUE;
}

//ɨ���߳�
DWORD WINAPI CleanThread(LPVOID lpVoid)
{
	Rubbish rs;
	for (DWORD i = 0; i < 11; i++)
	{
		rs.CleanRubbish(i);
	}
	return 0;
}

//��������
VOID Rubbish::CleanRubbish(int CleanType)
{
	switch (CleanType)
	{
	case CLEAN_SYSTEM_TEMP:
		CleanSystemTemp();
		break;
	case CLEAN_RUBBISH_STATION:
		CleanRubbishStation();
		break;
	case CLEAN_RUN_HISTORY:
		CleanRunHistory();
		break;
	case CLEAN_DOCUMENT_HISTORY:
		CleanDocumentHistory();
		break;
	case CLEAN_PREVUSER_HISTORY:
		CleanPrevUserHistory();
		break;
	case CLEAN_FILEFIND_HISTORY:
		CleanFileFindHistory();
		break;
	case CLEAN_INTERNET_TEMP:
		CleanInternetTemp();
		break;
	case CLEAN_INTERNET_COOKIE:
		CleanInternetCookie();
		break;
	case CLEAN_ADDRBAR_HISTORY:
		CleanAddressBarHistory();
		break;
	case CLEAN_PASSWORD_HISTORY:
		CleanPasswordHistory();
		break;
	case CLEAN_BROWSEADDR_HISTORY:
		CleanBrowseAddressHistory();
		break;
	}
}

//����ϵͳ��ʱ�ļ�
VOID Rubbish::CleanSystemTemp()
{
	WCHAR TempPath[MAX_PATH];
	//��API����������ʱ�ļ�Ŀ¼·��
	BOOL bSuccess = GetTempPath(MAX_PATH, TempPath);

	//ZeroMemory(TempPath, sizeof(TempPath));

	if (bSuccess)//�õ�ϵͳ��ʱĿ¼
	{
		EmptyDirectory(TempPath, TRUE);
	}
}

//�����ʱ�ļ���
BOOL Rubbish::EmptyDirectory(LPCTSTR szTempPath,
	BOOL bDeleteDesktopIni /*= FALSE*/, BOOL bWipeIndexDat /*= FALSE*/)
{
	WIN32_FIND_DATA wfd;
	HANDLE FindFileHandle = NULL;

	DWORD dwAttributes = 0;
	//�����ʼ��������lstrcat��������Ϊδ��ʼ������Ϊ�ַ���
	WCHAR wzFileFullPath[MAX_PATH] = {0};
	lstrcat(wzFileFullPath, (WCHAR*)szTempPath);
	lstrcat(wzFileFullPath, L"*.*");

	if ((FindFileHandle = FindFirstFile(wzFileFullPath, &wfd)) == INVALID_HANDLE_VALUE)
		return FALSE;
	do
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, TEXT("desktop.ini")) == 0))
		{
			continue;
		}

		WCHAR wzFilePath[MAX_PATH] = {0};
		lstrcat(wzFilePath, (WCHAR*)szTempPath);
		lstrcat(wzFilePath, wfd.cFileName);

		RUBBISH_FILE_INFO RubbishFileInfo = {0};
		//�ж��Ƿ���ɨ���ļ�������� �������List
		if (m_IsScanFile)
		{
			//���뵽List
			if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				//�����Ŀ¼����ݹ���ã��о��¼�Ŀ¼
				EmptyDirectory(wzFilePath);
				continue;
			}
			//�ļ�·��
			lstrcat(RubbishFileInfo.wzFilePath, wzFilePath);
			//�ļ���С
			ShowFileSize(wfd.nFileSizeLow, RubbishFileInfo.wzFileSize);
			m_RubbishFiles.push_back(RubbishFileInfo);
			setlocale(LC_ALL, "");
			printf("�ļ�·����%S\r\n", RubbishFileInfo.wzFilePath);
			printf("�ļ���С��%S\r\n", RubbishFileInfo.wzFileSize);
			//���������ļ��Ĵ�С ������ʾ�ж���
			m_AllRubbishSize += wfd.nFileSizeLow;
			continue;
		}

		//ȥ��ֻ������
		dwAttributes = GetFileAttributes(wzFilePath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(wzFilePath, dwAttributes);
		}
		//�ݹ��ļ����е��ļ�
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//����ļ���С����0˵�����ļ��� �ݹ��ѯ
			EmptyDirectory(wzFilePath);
			//ɾ�����ļ����е��ļ�����ɾ���ļ���
			RemoveDirectory(wzFilePath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, TEXT("index.dat")) == 0)
			{
				WipeFile(szTempPath, wfd.cFileName);
			}
			//���������ļ��Ĵ�С ������ʾ�ж���
			m_AllRubbishSize += wfd.nFileSizeLow;
			if (!DeleteFile(wzFilePath))
			{
				m_AllRubbishSize -= wfd.nFileSizeLow;
			}
		}
	} while (FindNextFile(FindFileHandle, &wfd));
	FindClose(FindFileHandle);
	//PrintRubbishInfo();
	return TRUE;
}


BOOL Rubbish::ShowFileSize(DOUBLE dwSize, WCHAR* wzFileSize)
{
	
	if (dwSize<1024)
	{
		//B
		swprintf(wzFileSize, L"%.2f B", dwSize);
		return TRUE;
	}
	else if (dwSize<1024 * 1024)
	{
		//KB
		swprintf(wzFileSize, L"%.2f KB", dwSize/1024.0f);
		return TRUE;
	}
	else if (dwSize<1024 * 1024 * 1024)
	{
		//MB
		swprintf(wzFileSize, L"%.2f MB", dwSize / 1024.0f/1024.0f);
		return TRUE;
	}
	else if (dwSize>1024 * 1024 * 1024)
	{
		//GB
		swprintf(wzFileSize, L"%.2f GB", dwSize / 1024.0f / 1024.0f/1024.0f);
		return TRUE;
	}
	return FALSE;
}

//�����ļ�����
BOOL Rubbish::WipeFile(LPCTSTR wzDirectory, LPCTSTR wzFilePath)
{

	HANDLE	FileHandle;
	DWORD	dwSize;
	DWORD	dwWrite;
	TCHAR	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	WCHAR* wzFileFullPath = new WCHAR(MAX_PATH);
	lstrcat(wzFileFullPath, (WCHAR*)wzDirectory);
	lstrcat(wzFileFullPath, L"\\");
	lstrcat(wzFileFullPath, (WCHAR*)wzFilePath);
	

	FileHandle = CreateFile(wzFileFullPath, GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	dwSize = GetFileSize(FileHandle, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(FileHandle, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(FileHandle, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(FileHandle, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(FileHandle);
	return TRUE;
}


VOID Rubbish::PrintRubbishInfo()
{
	
	printf("----------------------������Ϣ----------------------\r\n");
	WCHAR AllRubbishSize[0x20];
	ShowFileSize(m_AllRubbishSize, AllRubbishSize);
	printf("������С��%S\r\n", AllRubbishSize);
	vector<RUBBISH_FILE_INFO>::iterator RubbishInfo;
	for (RubbishInfo = m_RubbishFiles.begin(); RubbishInfo != m_RubbishFiles.end(); ++RubbishInfo)
	{
		setlocale(LC_ALL, "");
		printf("***************************\r\n");
		printf("�ļ�·����%S\r\n",RubbishInfo->wzFilePath);
		printf("�ļ���С��%S\r\n", RubbishInfo->wzFileSize);
		printf("***************************\r\n");
	}
	printf("----------------------------------------------------\r\n");
}

//�������վ
VOID Rubbish::CleanRubbishStation()
{
	//�����ɨ��ģʽ��ֱ�Ӳ�����
	if (m_IsScanFile)
	{
		return;
	}		
	//���ָ���������ϵĻ���վ����һ������ʾ���ھ��������ڶ�����ΪNULL�����������л���վ��
	//�������� ��ʾȷ��ɾ���Ի���|����ʾɾ������|������ɺ󲻻Ქ������
	SHEmptyRecycleBin(NULL, NULL,
		SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//�������м�¼
VOID Rubbish::CleanRunHistory()
{
	if (m_IsScanFile)
	{
		return;
	}
	//��ע�����ɾ�������Ӽ�������	
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
}

//��������ĵ���¼
VOID Rubbish::CleanDocumentHistory()
{
	WCHAR DocPath[MAX_PATH] = { 0 };

	//������CSIDL��ʾ�������ļ���·��
	BOOL bOk = SHGetSpecialFolderPath(NULL,
		DocPath, CSIDL_RECENT, FALSE);
	
	if (bOk)
	{
		EmptyDirectory(DocPath);
	}
	if (m_IsScanFile)
	{
		return;
	}	
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs"));
}

//�����ϴ��û���¼��¼
VOID Rubbish::CleanPrevUserHistory()
{
	if (m_IsScanFile)
		return;
	//ɾ��ע����Ӽ���ֵ
	SHDeleteValue(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		TEXT("DefaultUserName"));
	SHDeleteValue(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		TEXT("AltDefaultUserName"));
	SHDeleteValue(HKEY_LOCAL_MACHINE,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Winlogon"),
		TEXT("DefaultUserName"));
}

//�����ļ����Ҽ�¼
VOID Rubbish::CleanFileFindHistory()
{
	if (m_IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Doc Find Spec MRU"));
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\Explorer Bars\\{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\ContainingTextMRU"));
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\Explorer Bars\\{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\FilesNamedMRU"));
}

VOID Rubbish::CleanInternetTemp()
{
	WCHAR InternetPath[MAX_PATH] = { 0 };
	if (!m_IsScanFile)
	{
		DeleteUrlCache(FILES);
	}
	//SHGetSpecialFolderPath ��ȡϵͳָ��ϵͳ·��
	BOOL bOk = SHGetSpecialFolderPath(NULL,
		InternetPath, CSIDL_INTERNET_CACHE, FALSE);

	if (bOk)
	{  //�õ���ʱĿ¼���������.
		EmptyDirectory(InternetPath);
	}
}

//����Internet Cookie
VOID Rubbish::CleanInternetCookie()
{
	WCHAR CookiePath[MAX_PATH] = { 0 };
	DeleteUrlCache(COOKIES);
	BOOL bOk = SHGetSpecialFolderPath(NULL,
		CookiePath, CSIDL_COOKIES, FALSE);
	
	if (bOk)
	{//�õ�Ŀ¼�������
		EmptyDirectory(CookiePath);
	}
}

//�����ַ����ʷ��¼
VOID Rubbish::CleanAddressBarHistory()
{
	if (m_IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}

//���������¼
VOID Rubbish::CleanPasswordHistory()
{
	if (m_IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}

//���������ַ��¼
VOID Rubbish::CleanBrowseAddressHistory()
{
	if (m_IsScanFile)
		return;
	HRESULT HResult;//����ֵ
	WCHAR HistoryPath[MAX_PATH] = { 0 };
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	//����ѡ����Ķ�����ָ����ѡ����ID�����ID.
	HResult = CoCreateInstance(CLSID_CUrlHistory, NULL,
		CLSCTX_INPROC_SERVER, IID_IUrlHistoryStg2,
		(VOID**)&pUrlHistoryStg2);
	if (SUCCEEDED(HResult))
	{
		HResult = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}

	delete pUrlHistoryStg2;

	// ���������벻����
	// ��������ģ������������ܹ���.
	//����õ�System32�ļ���
	GetWindowsDirectory(HistoryPath, MAX_PATH);
	lstrcat(HistoryPath,L"\\History");
	EmptyDirectory(HistoryPath, FALSE, TRUE);

	BOOL bSuccess = SHGetSpecialFolderPath(NULL, HistoryPath, CSIDL_HISTORY, FALSE);
	if (bSuccess)
	{
		EmptyDirectory(HistoryPath, FALSE, TRUE);
	}
}


//���Url����
BOOL Rubbish::DeleteUrlCache(DEL_CACHE_TYPE Type)
{
	BOOL bRet = FALSE;   //����ֵ
	HANDLE EntryHandle;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	//���INTERNET_CACHE_ENTRY_INFO�Ĵ�С,��һ�ε��ô�NULL����������С
	EntryHandle = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	//������Ӧ��С�Ļ�����
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) malloc(dwEntrySize);
	//��ý���ڵľ��
	EntryHandle = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);

	if (EntryHandle)
	{
		do
		{
			if (Type == FILES &&
				!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			else if (Type == COOKIES &&
				(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			dwEntrySize = 0;
			FindNextUrlCacheEntry(EntryHandle, NULL, &dwEntrySize);
			free(lpCacheEntry);
			lpCacheEntry = NULL;
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)malloc(dwEntrySize);
		} while (FindNextUrlCacheEntry(EntryHandle, lpCacheEntry, &dwEntrySize));

		bRet = TRUE;
	}

	if (lpCacheEntry)
	{
		free(lpCacheEntry);
		lpCacheEntry = NULL;
	}
	return bRet;
}