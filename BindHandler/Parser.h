#pragma once
#include "DllProvider.h"
#include <locale>
#include <codecvt>
#include <string>
#include <memory>

class Parser
{
	int argc = 0;
	char** args = nullptr;

public:
	Parser() = delete;
	Parser(int argc, char** args) :
		argc(argc), args(args)
	{}

	Parser(const Parser&) = delete;
	Parser(Parser&&) = delete;
	Parser& operator =(const Parser&) = delete;
	Parser& operator =(Parser&&) = delete;
	~Parser() = default;

	void Get(void) const;
	void Set(void) const;
	void Edit(void) const;
	void Remove(void) const;
	void List(void);
};

