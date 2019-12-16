#include <Windows.h>
#include <stdio.h>

#pragma warning(disable : 4996)

#define BUFFER_LEN 1024

TCHAR szName[] = TEXT("Global\\AboutSystem");

int main() {
	HANDLE hMapFile;
	BOOL bResult;
	DWORD bufCharCount = BUFFER_LEN;
	PCHAR lpBuffer = NULL;
	char buffer[BUFFER_LEN];
	char computerName[BUFFER_LEN];
	char userName[BUFFER_LEN];
	size_t szBuffer = sizeof(buffer);

	//�������� ��� ����������
	bufCharCount = BUFFER_LEN;
	GetComputerName(computerName, &bufCharCount);

	//�������� ��� ������������
	bufCharCount = BUFFER_LEN;
	GetUserName(userName, &bufCharCount);

	//��������� ������ ��
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	printf("%d.%d.%d\n", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

	//������� ��� ����������, ��� ������������ � ������ �� � �����
	snprintf(buffer, sizeof buffer, "Computer Name: %s\nUser Name: %s\nOS Version: %d.%d.%d", computerName, userName, osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

	//������� ������ ������������� �����
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		256,
		szName);

	if (hMapFile == FALSE) {
		printf("CreateFileMapping Failed with error %d\n", GetLastError());
	}

	printf("CreateFileMapping: Success.\n");

	//�������� ��������� �� ������� ����� � ������������
	lpBuffer = (PCHAR)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);

	if (lpBuffer == NULL) {
		printf("MapViewOfFile failed with error %d\n", GetLastError());
	}

	printf("MapViewOfFile: Success.\n");

	//�������� ��� �����
	CopyMemory(lpBuffer, buffer, szBuffer);

	//���������� �����������
	bResult = UnmapViewOfFile(lpBuffer);

	if (bResult == FALSE) {
		printf("UnmapViewOfFile failed with error %d.\n", GetLastError());
	}

	printf("UnmapViewOfFile: Success.\n");

	system("PAUSE");
	return 0;
}