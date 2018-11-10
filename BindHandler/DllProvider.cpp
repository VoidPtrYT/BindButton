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
	if (!this->sizePairs < index)
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

	std::wcout << L"Is Caps: " << (bool)this->pairs[id].isCaps << std::endl;
	std::wcout << L"Is Shift: " << (bool)this->pairs[id].isShift << std::endl;
	std::wcout << L"Is Control: " << (bool)this->pairs[id].isControl << std::endl;
	std::wcout << L"Is Alt: " << (bool)this->pairs[id].isAlt << std::endl;

	std::wcout << L"Command: " << (this->pairs[id].strParams == nullptr ?
		L"Empty" : this->pairs[id].strParams) << std::endl;
}

DllProvider::~DllProvider(void)
{
	if (!this->hDll ||
		this->hDll != INVALID_HANDLE_VALUE)
		FreeLibrary(this->hDll);
	if (this->pairs)
	{
		free(this->pairs);
		this->pairs = nullptr;
	}
}
