// BindHandler.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Pair.h"
#include "DllProvider.h"
#include "Parser.h"
#include <string>

//g(get) id
//s(set) buttons, command
//e(edit) id, comand
//r(remove) id
//l(list)
//h, ?, help

DllProvider* DllProvider::instance = nullptr;

void Help(void);
void ShowError(void);
void ShowErrorLoadDLL(void);

int main(int argc, char** args)
{
	std::boolalpha(std::wcout);
	Parser p(argc, args);
	try
	{
		if (argc == 1)
		{
			Help();
			return 0;
		}

		if (DllProvider::getInstance()->bad())
		{
			ShowErrorLoadDLL();
			return 2;
		}

		std::string strParam(args[1]);
		if (strParam.length() != 2)
		{
			if (!strParam.compare("-help") ||
				!strParam.compare("/help"))
			{
				Help();
				return 0;
			}
			ShowError();
			return 1;
		}
		switch (strParam[1])
		{
		case '?':
		case 'h':
			Help();
			break;
		case 'g':
			p.Get();
			break;
		case 's':
			p.Set();
			DllProvider::getInstance()->Serialize();
			break;
		case 'e':
			p.Edit();
			DllProvider::getInstance()->Serialize();
			break;
		case 'r':
			p.Remove();
			DllProvider::getInstance()->Serialize();
			break;
		case 'l':
			p.List();
			break;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}
	catch (...)
	{
		std::wcout << L"Unhandled exception" << std::endl;
	}
    return 0;
}

void Help(void)
{
	std::wcout << L"List of commands:" << std::endl;
	std::wcout << L"\t-g <id> - get data by id" << std::endl;
	std::wcout << L"\t-s <id button> <isCaps> <isShift> <isControl> <isAlt> <command> - add\nbind" << std::endl;
	std::wcout << L"\t-e <id> <command> - edit bind" << std::endl;
	std::wcout << L"\t-r <id> - remove bind by id" << std::endl;
	std::wcout << "\t-l - get list data" << std::endl;
	std::wcout << "\t-h, -?, -help - get help about programm" << std::endl;
	std::wcout << "\tBy VoidPtr (c)2018" << std::endl;
}

void ShowError(void)
{
	std::wcout << L"Incorrect param!" << std::endl;
	std::wcout << L"Use -h, -?, -help for help" << std::endl;
}

void ShowErrorLoadDLL(void)
{
	std::wcout << L"Error of load DLL!" << std::endl;
	std::wcout << L"Try reinstall programm" << std::endl;
}