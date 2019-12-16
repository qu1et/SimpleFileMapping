#include <Windows.h>
#include <stdio.h>

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

	//Получаем имя компьютера
	bufCharCount = BUFFER_LEN;
	GetComputerName(computerName, &bufCharCount);

	//Получаем имя пользователя
	bufCharCount = BUFFER_LEN;
	GetUserName(userName, &bufCharCount);

	//Заносим имя компьютера и имя пользователя в буфер
	snprintf(buffer, sizeof buffer, "Computer Name: %s\nUser Name: %s", computerName, userName);

	/*Получение версии ОС (Закрыто на ремонт)
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionExA(&osvi);
	*/

	//Создаем объект отображенного файла
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

	//Получаем указатель на участок файла с отображением
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

	//Копируем наш буфер
	CopyMemory(lpBuffer, buffer, szBuffer);

	//Прекращаем отображение
	bResult = UnmapViewOfFile(lpBuffer);

	if (bResult == FALSE) {
		printf("UnmapViewOfFile failed with error %d.\n", GetLastError());
	}

	printf("UnmapViewOfFile: Success.\n");

	system("PAUSE");
	return 0;
}