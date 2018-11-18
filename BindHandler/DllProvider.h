#pragma once
#include "stdafx.h"
#include "Pair.h"

#define PATH L"FileSystemProvider.dll"
#define SERIALIZE "Serialize"
#define DESERIALIZE "Deserialize"
#define IS_FILE_EDIT "IsFileEdit"

class DllProvider
{
	HMODULE hDll = nullptr;
	FARPROC serialize = nullptr;
	FARPROC deserialize = nullptr;
	LARGE_INTEGER li = { 0 };
	PPAIR pairs = nullptr;
	size_t sizePairs = 0;
	bool isBad = false;

	DllProvider(void);
	static DllProvider* instance;

	void ListEmptyMessage(void) const;
	void InvalidIndex(void) const;
	bool IsError(size_t index) const;

public:

	static DllProvider* getInstance(void);

	bool Serialize(void) const;
	bool Deserialize(void);

	void get(size_t id) const;
	void set(const LPARAM* pLparam, size_t cntParams, BOOL isCaps, BOOL isShift, BOOL isAlt, BOOL isControl, LPCWSTR strParam);
	void edit(size_t id, LPCWSTR strParam);
	void remove(size_t id);
	void list(void) const;

	inline bool bad(void) const throw()
	{
		return this->isBad;
	}

	~DllProvider(void);
};

