#pragma once
#include <Windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

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
//	HANDLE hFile = CreateFile(PATH,
//		GENERIC_WRITE,
//		FILE_SHARE_READ,
//		NULL,
//		CREATE_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//		return FALSE;
//
//	DWORD iCountWrited = 0;
//	if (!WriteFile(hFile, &size, sizeof(DWORD), &iCountWrited, NULL) &&
//		sizeof(DWORD) != iCountWrited)
//		goto err;
//	for (DWORD i = 0; i < size; ++i)
//	{
//		if (!WriteFile(hFile, &pairArr[i].sizeArr, sizeof(DWORD), &iCountWrited, NULL) &&
//			sizeof(DWORD) != iCountWrited)
//			goto err;
//		//if (!WriteFile(hFile, pairArr[i].arr, sizeof(DWORD)*pairArr[i].sizeArr, &iCountWrited, NULL))//
//			//goto err;
//		for (size_t j = 0; i < pairArr[i].sizeArr; ++j)
//		{
//			if (!WriteFile(hFile, &pairArr[i].arr[j], sizeof(DWORD), &iCountWrited, NULL) &&
//				sizeof(DWORD) != iCountWrited)
//				goto err;
//		}
//
//		ByteData bd = { 0 };
//		bd.arr[0] = pairArr[i].isCaps;
//		bd.arr[1] = pairArr[i].isShift;
//		bd.arr[2] = pairArr[i].isControl;
//		bd.arr[3] = pairArr[i].isAlt;
//		if (!WriteFile(hFile, &bd.val, sizeof(INT32), &iCountWrited, NULL) &&
//			sizeof(INT32) != iCountWrited)
//			goto err;
//		DWORD sizeStr = wcslen(pairArr[i].strParams);
//		if (!WriteFile(hFile, &sizeStr, sizeof(DWORD), &iCountWrited, NULL) && 
//			sizeof(DWORD) != iCountWrited)
//			goto err;
//		if (!WriteFile(hFile, pairArr[i].strParams, sizeStr * sizeof(WCHAR), &iCountWrited, NULL) &&
//			sizeStr * sizeof(WCHAR) != iCountWrited)
//			goto err;
//	}
//
//	CloseHandle(hFile);
//	return TRUE;
//err:
//	CloseHandle(hFile);
//	return FALSE;
	if (pairArr == NULL)
		return FALSE;
	if (size == 0)
		return TRUE;

	FILE* f = NULL;
	if (_wfopen_s(&f, PATH, L"w"))
		return FALSE;

	fwprintf(f, L"%d\n\n", size);
	for (size_t i = 0; i < size; ++i)
	{
		fwprintf(f, L"%d %d\n", pairArr[i].sizeArr, wcslen(pairArr[i].strParams));
		for (size_t j = 0; j < pairArr[i].sizeArr; ++j)
		{
			fwprintf(f, L"%d", pairArr[i].arr[j]);
			fwprintf(f, j == pairArr[i].sizeArr - 1 ? L"\n" : L" ");
		}
		fwprintf(f, L"%s\n", pairArr[i].strParams);
		fwprintf(f, L"%d %d %d %d\n\n",
			pairArr[i].isCaps,
			pairArr[i].isShift,
			pairArr[i].isControl,
			pairArr[i].isAlt);
	}

	fclose(f);
	return TRUE;
}
BOOL EXPORT Deserialize(PPAIR pairArr, LPDWORD size)
{
//	if (!size || !pairArr)
//	{
//		for (DWORD i = 0; i < *size; ++i)
//		{
//			free(pairArr[i].arr);
//			free(pairArr[i].strParams);
//		}
//		*size = 0;
//		free(pairArr);
//	}
//
//	HANDLE hFile = CreateFile(PATH,
//		GENERIC_READ,
//		FILE_SHARE_READ,
//		NULL,
//		OPEN_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//		return FALSE;
//
//	DWORD iCountReaded = 0;
//	if (!ReadFile(hFile, size, sizeof(DWORD), &iCountReaded, NULL) &&
//		sizeof(DWORD) != iCountReaded)
//		goto err;
//	pairArr = (PPAIR)calloc(*size, sizeof(Pair));
//	for (DWORD i = 0; i < *size; ++i)
//	{
//		if (!ReadFile(hFile, &pairArr[i].sizeArr, sizeof(DWORD), &iCountReaded, NULL) || iCountReaded != sizeof(DWORD))
//			goto err;
//		pairArr[i].arr = (LPDWORD)calloc(pairArr[i].sizeArr, sizeof(DWORD));
//		//if (!ReadFile(hFile, pairArr[i].arr, sizeof(DWORD) * pairArr[i].sizeArr, &iCountReaded, NULL) || iCountReaded != sizeof(DWORD) * pairArr[i].sizeArr)
//		//	goto err;
//		for (size_t j = 0; j < pairArr[i].sizeArr; ++j)
//		{
//			if (!ReadFile(hFile, &pairArr[i].arr[j], sizeof(DWORD), &iCountReaded, NULL) &&
//				sizeof(DWORD) != iCountReaded)
//				goto err;
//		}
//
//		ByteData bd = { 0 };
//		if (!ReadFile(hFile, &bd.val, sizeof(INT32), &iCountReaded, NULL) || iCountReaded != sizeof(INT32))
//			goto err;
//		pairArr[i].isCaps = bd.arr[0];
//		pairArr[i].isShift = bd.arr[1];
//		pairArr[i].isControl = bd.arr[2];
//		pairArr[i].isAlt = bd.arr[3];
//
//		DWORD sizeStr = 0;
//		if (!ReadFile(hFile, &sizeStr, sizeof(DWORD), &iCountReaded, NULL) || iCountReaded != sizeof(DWORD))
//			goto err;
//		pairArr[i].strParams = (LPWSTR)calloc(sizeStr + 1, sizeof(WCHAR));
//		if (!ReadFile(hFile, pairArr[i].strParams, sizeStr * sizeof(WCHAR), &iCountReaded, NULL) || iCountReaded != sizeStr * sizeof(WCHAR))
//			goto err;
//	}
//	CloseHandle(hFile);
//	return TRUE;
//err:
//	CloseHandle(hFile);
//	return FALSE;
	if (pairArr != NULL && *size != 0)
	{
		for (size_t i = 0; i < *size; ++i)
		{
			free(pairArr[i].arr);
			free(pairArr[i].strParams);
		}
		free(pairArr);
		*size = 0;
	}

	FILE* f = NULL;
	if (_wfopen_s(&f, PATH, L"r"))
		return FALSE;

	fwscanf_s(f, L"%d\n\n", size);
	pairArr = (PPAIR)calloc(*size, sizeof(Pair));
	for (size_t i = 0; i < *size; ++i)
	{
		DWORD sizeStr = 0;
		fwscanf_s(f, L"%d %d\n", &pairArr[i].sizeArr, &sizeStr);
		pairArr[i].arr = (LPDWORD)calloc(pairArr[i].sizeArr, sizeof(DWORD));
		pairArr[i].strParams = (LPWSTR)calloc(sizeStr + 1, sizeof(WCHAR));

		for (size_t j = 0; j < pairArr[i].sizeArr; ++j)
		{
			fwscanf_s(f, L"%d", &pairArr[i].arr[j]);
			fwscanf_s(f, j == pairArr[i].sizeArr - 1 ? L"\n" : L" ");
		}
		fgetws(pairArr[i].strParams, sizeStr, f);
		//fwscanf_s(f, L"%s\n", pairArr[i].strParams);
		int iArr[4] = { 0 };
		fwscanf_s(f, L"%d %d %d %d\n\n",
			&iArr[0],
			&iArr[1],
			&iArr[2],
			&iArr[3]);
		pairArr[i].isCaps = iArr[0] == 0 ? FALSE : TRUE;
		pairArr[i].isShift = iArr[1] == 0 ? FALSE : TRUE;
		pairArr[i].isControl = iArr[2] == 0 ? FALSE : TRUE;
		pairArr[i].isAlt = iArr[3] == 0 ? FALSE : TRUE;
	}

	fclose(f);
	return TRUE;
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