#include "stdafx.h"
#include "TagView.h"

TagView * TagView::create(HWND hParent, HINSTANCE hInst, int id, Point position, Size size)
{
	auto lv = new (std::nothrow) TagView();
	lv->_hParent = hParent;
	lv->_hInst = hInst;
	lv->_id = id;
	lv->_position = position;
	lv->_size = size;

	lv->_hWnd = CreateWindow(WC_LISTVIEWW, L"List View", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_ICON, position.X, position.Y, size.Width, size.Height, hParent, (HMENU)id, hInst, NULL);
	
	return lv;
}

void TagView::init()
{
	LVCOLUMN column;
	column.cx = _size.Width;
	column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	column.fmt = LVCFMT_LEFT;
	column.pszText = L"Tag";
	ListView_InsertColumn(_hWnd, 0, &column);


	_hImage = ImageList_Create(16, 16, ILC_COLOR16 | ILC_MASK, 0, 0);
	ListView_SetImageList(_hWnd, _hImage, LVSIL_SMALL);
	HICON hIco = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_TAG));
	ImageList_AddIcon(_hImage, hIco);
	/*
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON2));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON3));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON4));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON5));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON6));
	ImageList_AddIcon(_hImage, hIco);
	ListView_SetTextColor(_hwnd, RGB(0, 174, 239));*/
	ListView_SetExtendedListViewStyle(_hWnd, LVS_EX_FULLROWSELECT | LVS_EX_TRANSPARENTBKGND);
}

void TagView::addChild(Tag* tag, int index)
{
	LVITEM i;
	i.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	i.iSubItem = 0;
	WCHAR buffer[1000];
	wcscpy(buffer, tag->name.c_str());
	WCHAR buffer2[10];
	wsprintf(buffer2, L" (%d)", tag->getCount());
	wcscat(buffer, buffer2);
	i.pszText = buffer;
	i.iImage = 0;
	int* iIndex = new int;
	*iIndex = index;
	i.lParam = (LPARAM)iIndex;
	i.cchTextMax = 255;
	i.iItem = index;
	ListView_InsertItem(_hWnd, &i);
}

void TagView::onAddTag()
{
	clear();
	auto tagList = _priorityTag->getTagList();
	int i = 0;
	for (auto tag : tagList)
	{
		addChild(tag, i);
		i++;
	}
}

void TagView::onSearchTag(const wstring & tagSugg)
{
	clear();
	auto tagList = _priorityTag->getTagList();
	int i = 0;
	for (auto tag : tagList)
	{
		if (tag->name.find(tagSugg) == 0)
		{
			addChild(tag, i);
		}
		i++;
	}
}

HWND & TagView::getHWND()
{
	return _hWnd;
}

void TagView::setFont(HFONT hFont)
{
	SetWindowFont(_hWnd, hFont, true);
}

void TagView::setPriorityTag(PriorityTag * priorityTag)
{
	_priorityTag = priorityTag;
}

void TagView::clear()
{
	ListView_DeleteAllItems(_hWnd);
}

TagView::TagView()
{
}


TagView::~TagView()
{
}
