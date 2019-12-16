#include <Windows.h>
#include <stdio.h>

#define BUFFER_LEN 1024

TCHAR szFirstServerName[] = TEXT("Global\\AboutSystem");
TCHAR szSecondServerName[] = TEXT("Global\\AboutMemory");

void getInfoFromFirstServer();
void getInfoFromSecondServer();

int main() {
	int n = -1;
	while (n != 0) {
		Sleep(50);
		printf("\nWhat kind of information you want to receive?\n");
		printf("Computer Name, User Name, OS Version\n");
		printf("Percentage of physical memory used, percentage of virtual memory used\n");
		printf("Exit (the best choice)\n");
		printf("Input: ");
		scanf_s("%d", &n);

		switch (n) {
			case 1:
			{
				getInfoFromFirstServer();
				break;
			}
			case 2:
			{
				getInfoFromSecondServer();
				break;
			}
		}
	}

	return 0;
}

void getInfoFromFirstServer() {
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;

	//Step 1 Открываем файл отображенный файл
	hFileMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szFirstServerName
	);

	if (hFileMap == NULL) {
		printf("OpenFileMapping failed with error %d.\n");
	}

	printf("OpenFileMapping: Success.\n");

	//Step 2 Получаем данные
	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);

	if (lpBuffer == NULL) {
		printf("MapViewOfFile failed with error %d\n", GetLastError());
	}

	printf("MapViewOfFile: Success.\n");

	// Step 3 Выводим полученные данные 
	printf("%s\n", lpBuffer);

	// Step 4 Прекращаем отображение
	bResult = UnmapViewOfFile(lpBuffer);

	if (bResult == NULL) {
		printf("UnmapViewOfFile failed with error %d\n", GetLastError());
	}

	printf("UnmapViewOfFile: Success.\n");

	// Step 5 Close Handle

	CloseHandle(hFileMap);
}

void getInfoFromSecondServer() {
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;

	//Step OpenFileMapping
	hFileMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szSecondServerName
	);

	if (hFileMap == NULL) {
		printf("OpenFileMapping failed with error %d.\n");
	}

	printf("OpenFileMapping: Success.\n");

	//Step 2 MapViewOfFile

	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);

	if (lpBuffer == NULL) {
		printf("MapViewOfFile failed with error %d\n", GetLastError());
	}

	printf("MapViewOfFile: Success.\n");

	// Step 3 Reading Data from File Map Object

	printf("%s\n", lpBuffer);

	// Step 4 UnmapViewOfFile

	bResult = UnmapViewOfFile(lpBuffer);

	if (bResult == NULL) {
		printf("UnmapViewOfFile failed with error %d\n", GetLastError());
	}

	printf("UnmapViewOfFile: Success.\n");

	// Step 5 Close Handle

	CloseHandle(hFileMap);
}