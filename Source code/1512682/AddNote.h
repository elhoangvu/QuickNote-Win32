#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windows.h>
#include <string>
#include <vector>
#include "Note.h"
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
using namespace std;

class AddNote :public Note
{
public:
	void draw(HDC hdc, int offset);

	AddNote();
	~AddNote();
};

