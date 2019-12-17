#include <Windows.h>
#include <stdio.h>

#define BUFFER_LEN 1024

TCHAR szName[] = TEXT("Global\\AboutMemory");

int main() {
	HANDLE hMapFile;
	BOOL bResult;
	DWORD bufCharCount = BUFFER_LEN;
	PCHAR lpBuffer = NULL;
	char buffer[BUFFER_LEN];
	char physicalMemory[BUFFER_LEN];
	char virtualMemory[BUFFER_LEN];
	size_t szBuffer = sizeof(buffer);

	//Получаем информацию о памяти
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	//Физическая память
	double physMem = (double)(statex.ullTotalPhys - statex.ullAvailPhys) / statex.ullTotalPhys * 100;
	sprintf_s(physicalMemory, "%f", physMem);

	//Виртуальная память
	double virtMem = (double)(statex.ullTotalVirtual - statex.ullAvailVirtual) / statex.ullTotalVirtual * 100;
	sprintf_s(virtualMemory, "%f", virtMem);

	//Заносим инфу в буфер
	snprintf(buffer, sizeof buffer, "percent of physical memory used: %s\nPercent of virtual memory used: %s", physicalMemory, virtualMemory);

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
	else {
		printf("CreateFileMapping: Success.\n");
	}

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
	else {
		printf("MapViewOfFile: Success.\n");
	}

	//Копируем наш буфер
	CopyMemory(lpBuffer, buffer, szBuffer);

	//Прекращаем отображение
	bResult = UnmapViewOfFile(lpBuffer);

	if (bResult == FALSE) {
		printf("UnmapViewOfFile failed with error %d.\n", GetLastError());
	}
	else {
		printf("UnmapViewOfFile: Success.\n");
	}

	system("PAUSE");
	return 0;
}