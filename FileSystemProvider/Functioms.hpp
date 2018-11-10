#pragma once
#include <Windows.h>
#include <malloc.h>
#include <string.h>

#define PATH L".\\combination.data"
#define EXPORT __declspec(dllexport)

typedef struct Pair
{
	LPDWORD arr; //arr iKey not LPARAM
	DWORD sizeArr;
	BOOL isCaps, isShift, isControl, isAlt;

	LPWSTR strParams;
} Pair;
typedef Pair* PPAIR;

typedef union ByteData
{
	INT32 val;
	BOOL arr[4];
} ByteData;

BOOL EXPORT Serialize(const PPAIR pairArr, DWORD size);
BOOL EXPORT Deserialize(PPAIR pairArr, LPDWORD size);
DWORD EXPORT IsFileEdit(PLARGE_INTEGER sizeFile);

BOOL EXPORT Serialize(const PPAIR pairArr, DWORD size)
{
	HANDLE hFile = CreateFile(PATH,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD iCountWrited = 0;
	if (!WriteFile(hFile, &size, sizeof(DWORD), &iCountWrited, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	for (DWORD i = 0; i < size; ++i)
	{
		LPBYTE data = NULL;
		DWORD sizeBuffer = 0;
		sizeBuffer += sizeof(DWORD);
		sizeBuffer += sizeof(DWORD) * pairArr[i].sizeArr;
		sizeBuffer += 4 * sizeof(BOOL);
		sizeBuffer += wcslen(pairArr[i].strParams) * sizeof(WCHAR);
		data = (LPBYTE)calloc(sizeBuffer, sizeof(BYTE));
		DWORD iIndex = 0;

		memcpy(data + iIndex, &pairArr[i].sizeArr, sizeof(DWORD));
		iIndex += sizeof(DWORD);

		memcpy(data + iIndex, pairArr[i].arr, sizeof(DWORD) * pairArr[i].sizeArr);
		iIndex += sizeof(DWORD) * pairArr[i].sizeArr;

		ByteData bd;
		bd.arr[0] = pairArr[i].isCaps;
		bd.arr[1] = pairArr[i].isShift;
		bd.arr[2] = pairArr[i].isControl;
		bd.arr[3] = pairArr[i].isAlt;
		memcpy(data + iIndex, &bd, sizeof(INT32));
		iIndex += sizeof(INT32);

		memcpy(data + iIndex, pairArr[i].strParams, wcslen(pairArr[i].strParams) * sizeof(WCHAR));

		if (!WriteFile(hFile, data, sizeBuffer, &iCountWrited, NULL) || iCountWrited != sizeBuffer)
		{
			free(data);
			CloseHandle(hFile);
			return FALSE;
		}
		free(data);
	}

	CloseHandle(hFile);
	return TRUE;
}
BOOL EXPORT Deserialize(PPAIR pairArr, LPDWORD size)
{
	if (!size || !pairArr)
	{
		for (DWORD i = 0; i < *size; ++i)
		{
			free(pairArr[i].arr);
			free(pairArr[i].strParams);
		}
		size = 0;
		free(pairArr);
	}

	HANDLE hFile = CreateFile(PATH,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD iCountReaded = 0;
	if (!ReadFile(hFile, size, sizeof(DWORD), &iCountReaded, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	pairArr = (PPAIR)calloc(*size, sizeof(Pair));
	for (DWORD i = 0; i < *size; ++i)
	{
		if (!ReadFile(hFile, &pairArr[i].sizeArr, sizeof(DWORD), &iCountReaded, NULL) || iCountReaded != sizeof(DWORD))
			goto err;
		pairArr[i].arr = (LPDWORD)calloc(pairArr[i].sizeArr, sizeof(DWORD));
		if (!ReadFile(hFile, pairArr[i].arr, sizeof(DWORD) * pairArr[i].sizeArr, &iCountReaded, NULL) || iCountReaded != sizeof(DWORD) * pairArr[i].sizeArr)
			goto err;
		ByteData bd;
		if (!ReadFile(hFile, &bd, sizeof(ByteData), &iCountReaded, NULL) || iCountReaded != sizeof(ByteData))
			goto err;
		pairArr[i].isCaps = bd.arr[0];
		pairArr[i].isShift = bd.arr[1];
		pairArr[i].isControl = bd.arr[2];
		pairArr[i].isAlt = bd.arr[3];

		DWORD sizeStr = 0;
		if (!ReadFile(hFile, &sizeStr, sizeof(DWORD), &iCountReaded, NULL) || iCountReaded != sizeof(DWORD))
			goto err;
		pairArr[i].strParams = (LPWSTR)calloc(sizeStr + 1, sizeof(WCHAR));
		if (!ReadFile(hFile, pairArr[i].strParams, sizeStr * sizeof(WCHAR), &iCountReaded, NULL) || iCountReaded != sizeStr * sizeof(WCHAR))
			goto err;
	}
	CloseHandle(hFile);
	return TRUE;
err:
	CloseHandle(hFile);
	return FALSE;
}

//Return val: 0 - good, 1 - value edit, 2 - error
DWORD EXPORT IsFileEdit(PLARGE_INTEGER sizeFile)
{
	LARGE_INTEGER tmp;
	HANDLE hFile = CreateFile(PATH,
		GENERIC_READ,
		FILE_SHARE_DELETE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 2;

	GetFileSizeEx(hFile, &tmp);
	DWORD result = tmp.QuadPart == sizeFile->QuadPart ? 0 : 1;
	if (result)
		sizeFile->QuadPart = tmp.QuadPart;
	CloseHandle(hFile);
	return result;
}