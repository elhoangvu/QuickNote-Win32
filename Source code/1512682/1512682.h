#pragma once

#include "resource.h"
#include "NoteGridView.h"
#include "PriorityTag.h"
#include "TagView.h"
#include "PieChart.h"
#include <Shlwapi.h>
#include <Commctrl.h>
#include <ObjIdl.h>
#include <Windowsx.h>
#include <Shellapi.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

enum Scene
{
	VIEW_NOTE,
	VIEW_STATISTICS,
	VIEW_TAG,
};

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

NoteGridView* gNoteView;
PriorityTag* gPriorityTag;
TagView* gTagView;
vector<Note*>* gNoteList;
PieChart* gPieChart;
bool gIsDrew;
bool gIsExistDlg;
int yMaxScroll;

HHOOK hHook;
HWND hScrollWnd;
HWND hBtnStatistic;
HWND hSearchTag;
HWND hBtnNote;
HWND hWnd;
HFONT hFont;
HWND hEditDlg;
RECT gRectDlg;
Point gPrePos;
int gCurNote;
int gCurScene;
int gPreScene;

INT_PTR CALLBACK EditNoteProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onAddNote();
void onEditNote();
void onSetScrollInfo(bool isSetScrollDefault = false);
void LoadNotifyIcon(HWND hwnd);

void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);
LRESULT CALLBACK HookProc(int message, WPARAM wParam, LPARAM lParam);