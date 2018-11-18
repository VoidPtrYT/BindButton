#include "stdafx.h"
#include "DllProvider.h"


DllProvider::DllProvider()
{
	this->hDll = LoadLibrary(PATH);
	if (this->hDll == INVALID_HANDLE_VALUE)
	{
		this->isBad = true;
		return;
	}

	this->serialize = GetProcAddress(this->hDll, SERIALIZE);
	this->deserialize = GetProcAddress(this->hDll, DESERIALIZE);

	if (!this->serialize || !this->deserialize)
		this->isBad = true;
}

void DllProvider::ListEmptyMessage(void) const
{
	std::wcout << L"list of binds is empty!" << std::endl;
}

void DllProvider::InvalidIndex(void) const
{
	std::wcout << "Invalid index operation" << std::endl;
}

bool DllProvider::IsError(size_t index) const
{
	if (!this->pairs)
	{
		this->ListEmptyMessage();
		return false;
	}
	if (this->sizePairs >= index)
	{
		this->InvalidIndex();
		return false;
	}
	return true;
}

DllProvider* DllProvider::getInstance(void)
{
	if (!DllProvider::instance)
		DllProvider::instance = new DllProvider;

	return DllProvider::instance;
}

bool DllProvider::Serialize(void) const
{
	if (!this->pairs)
		return false;
	return reinterpret_cast
		<BOOL(*)(const PPAIR, DWORD)>
		(this->serialize)
		(this->pairs, this->sizePairs) == TRUE;
}

bool DllProvider::Deserialize(void)
{
	return reinterpret_cast
		<BOOL(*)(PPAIR, DWORD)>
		(this->deserialize)
		(this->pairs, this->sizePairs) == TRUE;
}

void DllProvider::get(size_t id) const
{
	if (!this->IsError(id))
		return;
	std::wcout << L"LPARAM: " << std::endl;
	for (size_t i = 0; i < this->pairs[id].sizeArr; ++i)
		std::wcout << L"\t" << i << ": " <<
		this->pairs->arr[i] << std::endl;

	std::wcout << L"Is Caps: " << (bool)(this->pairs[id].isCaps) << std::endl;
	std::wcout << L"Is Shift: " << (bool)(this->pairs[id].isShift) << std::endl;
	std::wcout << L"Is Control: " << (bool)(this->pairs[id].isControl) << std::endl;
	std::wcout << L"Is Alt: " << (bool)(this->pairs[id].isAlt) << std::endl;

	std::wcout << L"Command: " << (this->pairs[id].strParams == nullptr ?
		L"Empty" : this->pairs[id].strParams) << std::endl;
}

void DllProvider::set(const LPARAM* pLparam, size_t cntParams, BOOL isCaps, BOOL isShift, BOOL isAlt, BOOL isControl, LPCWSTR strParam)
{
	if (this->sizePairs == 0)
		this->pairs = (PPAIR)calloc(1, sizeof(Pair));
	else
		this->pairs = (PPAIR)realloc(this->pairs, (this->sizePairs + 1) * sizeof(Pair));

	++this->sizePairs;

	for (size_t i = 0; i < this->sizePairs; ++i)
	{
		if (this->pairs[i].arr == nullptr)
			continue;
		if (this->pairs[i].arr[0] == pLparam[0] &&
			this->pairs[i].sizeArr == cntParams)
		{
			bool isCorrect = false;
			for (size_t j = 1; j < this->sizePairs; ++j)
			{
				if (this->pairs[i].arr[j] != pLparam[j])
				{
					isCorrect = true;
					break;
				}
			}
			if (!isCorrect)
			{
				std::wcout << L"Element already exists!" << std::endl;
				std::wcout << L"Try remove or edit element by index: " << i << std::endl;
				return;
			}
		}
	}
	Pair& p = this->pairs[this->sizePairs - 1];
	p.sizeArr = cntParams;
	p.arr = (LPARAM*)calloc(p.sizeArr, sizeof(LPARAM));
	memcpy_s(p.arr, p.sizeArr * sizeof(LPARAM), pLparam, cntParams * sizeof(LPARAM));
	p.isCaps = isCaps;
	p.isShift = isShift;
	p.isAlt = isAlt;
	p.isControl = isControl;
	p.strParams = (LPWSTR)calloc(wcslen(strParam) + 1, sizeof(WCHAR));
	wcscpy_s(p.strParams, wcslen(strParam), strParam);
}

void DllProvider::edit(size_t id, LPCWSTR strParam)
{
	if (!this->IsError(id))
		return;
	if (strParam == nullptr ||
		!wcscmp(strParam, L""))
	{
		this->remove(id);
		return;
	}
	free(this->pairs[id].strParams);
	this->pairs[id].strParams = (LPWSTR)calloc(wcslen(strParam) + 1, sizeof(WCHAR));
	wcscpy_s(this->pairs[id].strParams, (wcslen(strParam) + 1), strParam);
}

void DllProvider::remove(size_t id)
{
	if (!this->IsError(id))
		return;
	free(this->pairs[id].arr);
	free(this->pairs[id].strParams);

	if (this->sizePairs == 1)
	{
		free(this->pairs);
		this->pairs = nullptr;
		return;
	}
	for (size_t i = id; i < this->sizePairs - 1; ++i)
		this->pairs[i] = this->pairs[i + 1];
	this->sizePairs -= 1;
	this->pairs = (PPAIR)realloc(this->pairs, this->sizePairs * sizeof(Pair));
}

void DllProvider::list(void) const
{
	if (this->sizePairs == 0)
	{
		this->ListEmptyMessage();
		return;
	}
	std::wcout << " id:\tButtons:\tCaps:\tShift:\tControl:\tAlt:\tCommand:" << std::endl;
	for (size_t i = 0; i < this->sizePairs; ++i)
	{
		std::wcout << ' ' << i << ' \t' << this->pairs[i].arr[0] << '\t'
			<< this->pairs[i].isCaps << '\t' << this->pairs[i].isControl
			<< '\t' << this->pairs[i].isAlt << '\t' << this->pairs[i].isAlt
			<< '\t' << this->pairs[i].strParams << std::endl;
		for (size_t j = 1; j < this->pairs[i].sizeArr; ++j)
			std::wcout << L"  \t" << this->pairs[i].arr[j];
	}
}

DllProvider::~DllProvider(void)
{
	this->Serialize();
	if (!this->hDll ||
		this->hDll != INVALID_HANDLE_VALUE)
		FreeLibrary(this->hDll);
	if (this->pairs)
	{
		free(this->pairs);
		this->pairs = nullptr;
	}
}