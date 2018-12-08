#pragma once

#include <vector>
#include <Commctrl.h>
#include <ObjIdl.h>
#include <windowsx.h>
#include <windows.h>
#include "Commctrl.h"
#include <new>
#include <gdiplus.h>
#include "Resource.h"
#include "PriorityTag.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class TagView
{
private:
	HWND _hParent;
	HINSTANCE _hInst;
	HWND _hWnd;
	HIMAGELIST _hImage;
	Point _position;
	Size _size;
	int _id;
	PriorityTag* _priorityTag;
public:
	static TagView* create(HWND hParent, HINSTANCE hInst, int id, Point position, Size size);
	void init();
	void addChild(Tag* tag, int index);
	void clear();

	void onAddTag();
	void onSearchTag(const wstring& tagSugg);

	HWND& getHWND();
	void setFont(HFONT hFont);
	void setPriorityTag(PriorityTag* priorityTag);

	TagView();
	~TagView();
};

