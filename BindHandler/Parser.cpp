#include "stdafx.h"
#include "Parser.h"

int Parser::ParseBool(char* str)
{
	if (strlen(str) != 1)
		return -1;

	if (str[0] == '1' || str[0] == 't' || str[0] == 'y')
		return 1;
	if (str[0] == '0' || str[0] == 'f' || str[0] == 'n')
		return 0;

	return -1;
}

void Parser::Get(void) const
{
	if (this->argc != 3)
	{
		std::wcout << L"Incorrect count of params!" << std::endl;
		std::wcout << L"\t-g <id> - get data by id" << std::endl;
		return;
	}

	size_t index = atoi(this->args[2]);
	if (index == 0 && this->args[2][0] != '0')
	{
		std::wcout << L"Incorrect value of index!" << std::endl;
		std::wcout << L"Index must be a numeric value" << std::endl;
		return;
	}
	DllProvider::getInstance()->get(index);
}

void Parser::Set(void) const
{
	if (this->argc < 7)
	{
		std::wcout << L"Error of count of params! This variant must have this params:" << std::endl;
		std::wcout << L"\t-s <id button> <isCaps> <isShift> <isControl> <isAlt> <command> - add\nbind" << std::endl;
		std::wcout << L"You can add more than one button. Separate codes by space" << std::endl;
		return;
	}
	size_t cntParams = this->argc - 6;
	LPARAM* arr = (LPARAM*)calloc(cntParams, sizeof(LPARAM));
	for (size_t i = 0; i < cntParams; ++i)
	{
		int id = atoi(this->args[2 + i]);
		if (id == 0 && this->args[2 + i][0] != '0')
		{
			std::wcout << L"Incorrect param of id button! Value must be integer!" << std::endl;
			free(arr);
			return;
		}
		arr[i] = (LPARAM)id;
	}

	int array[4] = { 0 };
	this->ParseBool(this->args[this->argc - 5]); // Çהוסü מרטבךא?!
}

void Parser::Edit(void) const
{
	if (this->argc != 4)
	{
		std::wcout << L"Incorrect count of params!" << std::endl;
		std::wcout << L"\t-e <id> <command> - edit bind" << std::endl;
		return;
	}
	size_t index = atoi(this->args[2]);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	std::wstring wstr = conv.from_bytes(this->args[3]);

	DllProvider::getInstance()->edit(index, wstr.data());
}

void Parser::Remove(void) const
{
	if (this->argc != 3)
	{
		std::wcout << L"Incorrect count of params!" << std::endl;
		std::wcout << L"\t-r <id> - remove bind by id" << std::endl;
		return;
	}

	size_t index = atoi(this->args[2]);
	if (index == 0 && this->args[2][0] != '0')
	{
		std::wcout << L"Incorrect value of index!" << std::endl;
		std::wcout << L"Index must be a numeric value" << std::endl;
		return;
	}
	DllProvider::getInstance()->remove(index);
}

void Parser::List(void)
{
	if (this->argc != 2)
	{
		std::wcout << L"This command must have only -l param" << std::endl;
		return;
	}
	DllProvider::getInstance()->list();
}
