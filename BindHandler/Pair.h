#pragma once
#include "stdafx.h"

typedef struct Pair
{
	LPARAM* arr;
	DWORD sizeArr;
	BOOL isCaps, isShift, isControl, isAlt;

	LPWSTR strParams;
} Pair;
typedef Pair* PPAIR;
