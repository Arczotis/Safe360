#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;
//��������Ϣ�ṹ��
typedef struct _SELFRUNNINGINFO
{
	TCHAR wzBootName[50];			//������
	TCHAR wzBootOrder[255];			//��������
	TCHAR wzHKEY[50];				//������Ϣ
	TCHAR wzBootPos[255];			//�Ӽ�
}SELFRUNNINGINFO, *PSELFRUNNINGINFO;


class SelfRunning
{
public:
	SelfRunning();
	~SelfRunning();
	void GetSelfRunningSoftware();

	void PrintInfo();

private:
	vector<SELFRUNNINGINFO> m_SelfRunningSoftware;
};




