// 1512682.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512682.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY1512682, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512682));

	MSG msg;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, 0, 0);
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	UnhookWindowsHookEx(hHook);
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512682));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512682);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, L"Quick Note", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	  CW_USEDEFAULT, 0, 1000, 650, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SCROLLINFO si;

	static int yMinScroll;       // minimum vertical scroll value 
	static int yCurrentScroll;   // current vertical scroll value 

	switch (message)
	{
	case ID_NOTIFY_CALLBACK:
	{
		switch (lParam)
		{
		case WM_LBUTTONUP:
			if (IsWindowVisible(hWnd) == false)
				ShowWindow(hWnd, TRUE);
			break;
		case WM_RBUTTONUP:
		{
			HMENU NotifyMenu = CreatePopupMenu();
			AppendMenu(NotifyMenu, MF_STRING, IDC_VIEW_NOTE, L"View Notes");
			AppendMenu(NotifyMenu, MF_STRING, IDC_VIEW_STATISTICS, L"View Statistics");
			AppendMenu(NotifyMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(NotifyMenu, MF_STRING, IDC_EXIT, L"Exit");
			POINT pos;
			GetCursorPos(&pos);
			TrackPopupMenu(NotifyMenu, TPM_RIGHTBUTTON, pos.x, pos.y, 0, hWnd, NULL);
		}
		break;
		}
		break;
	}
	case WM_CREATE:
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_DATE_CLASSES;

		InitCommonControlsEx(&icex);
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"Arial");

		if (hFont == NULL)
		{
			LOGFONT lf;
			GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			CreateFont(lf.lfHeight*1.6, lf.lfWidth*1.6,
				lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
				lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
				lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
				lf.lfPitchAndFamily, lf.lfFaceName);
		}


		yMinScroll = 0;
		yCurrentScroll = 0;
		yMaxScroll = 0;


		
		gPriorityTag = new PriorityTag();
		gTagView = TagView::create(hWnd, hInst, ID_TAG_VIEW, Point(22, 262), Size(248, 325));
		gTagView->init();
		gTagView->setFont(hFont);


		gNoteView = new NoteGridView();
		gNoteView->setHWND(hWnd);
		gNoteView->setPosition(Point(320, 45));
		gNoteView->setSizeInView(Size(630, 605));
		gNoteList = gNoteView->getNoteList();


		// The vertical scrolling range is defined by 
		// (bitmap_height) - (client_height). The current vertical 
		// scroll value remains within the vertical scrolling range. 

		RECT rect;
		GetClientRect(hWnd, &rect);

		yMaxScroll = gNoteView->getActualSize().Height;
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = 0;
		if (gNoteView->getActualSize().Height <= gNoteView->getSizeInView().Height)
		{
			si.nPage = rect.bottom;
			si.nMax = rect.bottom;
		}
		else
		{
			si.nPage = gNoteView->getSizeInView().Height * 0.85f;
			si.nMax = gNoteView->getActualSize().Height;
		}
		si.nPos = 0;


		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		///
		hBtnNote = CreateWindow(L"button", L"Ghi chú", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 20, 70, 250, 40, hWnd, (HMENU)ID_BTN_NOTE, hInst, 0);
		hBtnStatistic = CreateWindow(L"button", L"Xem thống kê", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 20, 120, 250, 40, hWnd, (HMENU)ID_BTN_STATISTIC, hInst, 0);
		hSearchTag = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE, 22, 222, 248, 28, hWnd, (HMENU)ID_SEARCH_TAG, hInst, 0);
		SetWindowFont(hSearchTag, hFont, true);

		LoadNotifyIcon(hWnd);
		gIsDrew = false;
		gIsExistDlg = false;

		//gPriorityTag->addTag(note);
		//note->initTagList(L"50,3,4");
		//gPriorityTag->addTag(note);
		gPieChart = PieChart::create(gPriorityTag);
		gPieChart->setPosition(Point(540, 160));
		gPieChart->setRadius(90);
		gCurScene = Scene::VIEW_NOTE;
		gPreScene = Scene::VIEW_NOTE;
		gTagView->setPriorityTag(gPriorityTag);
		gTagView->onAddTag();
		break;
	}
	HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem);
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_BTN_NOTE:
		{
			SetWindowText(hSearchTag, L"");
			if (gCurScene == Scene::VIEW_NOTE)
			{
				break;
			}
			gPreScene = Scene::VIEW_STATISTICS;
			gCurScene = Scene::VIEW_NOTE;
			gNoteView->initNoteList(gNoteList);
		
			RECT rect;
			rect.left = gNoteView->getPosition().X;
			rect.top = 0;
			rect.right = 1000;
			rect.bottom = gNoteView->getPosition().Y + gNoteView->getSizeInView().Height;
			onSetScrollInfo();
			InvalidateRect(hWnd, &rect, FALSE);

			break;
		}
		case ID_BTN_STATISTIC:
		{
			if (gCurScene == Scene::VIEW_STATISTICS)
			{
				break;
			}
			gPreScene = Scene::VIEW_NOTE;
			gCurScene = Scene::VIEW_STATISTICS;
			RECT rect;
			rect.left = gNoteView->getPosition().X;
			rect.top = 0;
			rect.right = 1000;
			rect.bottom = gNoteView->getPosition().Y + gNoteView->getSizeInView().Height;
			onSetScrollInfo(TRUE);
			InvalidateRect(hWnd, &rect, FALSE);

			break;
		}
		case ID_SEARCH_TAG:
		{
			if (wmEvent == EN_CHANGE)
			{
				WCHAR buffer[100];
				GetWindowText(hSearchTag, buffer, 100);
				gTagView->onSearchTag(buffer);
			}
			break;
		}
		case IDC_VIEW_NOTE:
		{
			SendMessage(hWnd, WM_COMMAND, ID_BTN_NOTE, 0);
			if (IsWindowVisible(hWnd) == false)
				ShowWindow(hWnd, TRUE);
			break;
		}
		case IDC_VIEW_STATISTICS:
		{
			SendMessage(hWnd, WM_COMMAND, ID_BTN_STATISTIC, 0);
			if (IsWindowVisible(hWnd) == false)
				ShowWindow(hWnd, TRUE);
			break;
		}
		case IDC_EXIT:
		{
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		ShowWindow(hWnd, FALSE);
		return TRUE;
	}
	case WM_PAINT:
	{
		PRECT prect;

		hdc = BeginPaint(hWnd, &ps);
		if (!gIsDrew)
		{
			gNoteView->setHDC(hdc);
			gNoteView->draw();

			gIsDrew = true;
			
		}
		if (gCurScene == Scene::VIEW_NOTE || gCurScene == Scene::VIEW_TAG)
		{
			gNoteView->setHDC(hdc);
			gNoteView->draw();
			auto graphic = new Graphics(hdc);
			SolidBrush brush1(Color(50, 0, 0, 0));
			graphic->FillRectangle(&brush1, 0, 5, 1000, 40);

			SolidBrush brush2(Color(255, 26, 188, 156));
			graphic->FillRectangle(&brush2, 0, 0, 1000, 41);

			SolidBrush brush3(Color(40, 26, 188, 156));
			graphic->FillRectangle(&brush3, 280, 70, 6, 517);
			SolidBrush brush4(Color(100, 26, 188, 156));
			graphic->FillRectangle(&brush4, 284, 70, 2, 517);

			Pen pen(Color(100, 26, 188, 156));
			graphic->DrawRectangle(&pen, 20, 260, 250, 327);
			graphic->DrawRectangle(&pen, 20, 220, 250, 30);
			FontFamily  fontFamily(L"Arial");
			Font        font(&fontFamily, 22, FontStyleBold, UnitPixel);
			PointF      pointF(10, 9);
			SolidBrush  solidBrush(Color(255, 255, 255, 255));

			graphic->DrawString(L"Quick Note", -1, &font, pointF, &solidBrush);

			Font        font2(&fontFamily, 15, FontStyleRegular, UnitPixel);
			pointF = PointF(20, 195);
			graphic->DrawString(L"Tìm kiếm thẻ:", -1, &font2, pointF, &brush2);
			delete graphic;
		}
		else
		{
			auto graphic = new Graphics(hdc);
			FontFamily  fontFamily(L"Arial");
			Font        font(&fontFamily, 18, FontStyleBold, UnitPixel);
			PointF      pointF(350, 70);
			SolidBrush  solidBrush(Color(255, 26, 188, 156));
			RECT rect;
			rect.left = 300;
			rect.top = 45;
			rect.right = 1000;
			rect.bottom = 650;
			FillRect(hdc, &rect, (HBRUSH)(COLOR_BTNFACE + 1));
			graphic->DrawString(L"Biểu đồ - thống kê:", -1, &font, pointF, &solidBrush);

			gPieChart->setHDC(hdc);
			gPieChart->draw(hWnd);
			delete graphic;
		}
		EndPaint(hWnd, &ps);

		break;
	}


	case WM_VSCROLL:
	{
		int xDelta = 0;
		int yDelta;     // yDelta = new_pos - current_pos 
		int yNewPos;    // new position 

		switch (LOWORD(wParam))
		{
			// User clicked the scroll bar shaft above the scroll box. 
		case SB_PAGEUP:
			yNewPos = yCurrentScroll - 50;
			break;

			// User clicked the scroll bar shaft below the scroll box. 
		case SB_PAGEDOWN:
			yNewPos = yCurrentScroll + 50;
			break;

			// User clicked the top arrow. 
		case SB_LINEUP:
			yNewPos = yCurrentScroll - 5;
			break;

			// User clicked the bottom arrow. 
		case SB_LINEDOWN:
			yNewPos = yCurrentScroll + 5;
			break;

			// User dragged the scroll box. 
		case SB_THUMBPOSITION:
			yNewPos = HIWORD(wParam);
			break;
		case SB_ENDSCROLL:
			yNewPos = yCurrentScroll;
			break;
		default:
			yNewPos = HIWORD(wParam);
			//yNewPos = yCurrentScroll;
			break;
		}

		// New position must be between 0 and the screen height. 
		yNewPos = max(0, yNewPos);
		yNewPos = min(yMaxScroll, yNewPos);

		// If the current position does not change, do not scroll.
		if (yNewPos == yCurrentScroll)
			break;

		// Determine the amount scrolled (in pixels). 
		yDelta = yNewPos - yCurrentScroll;

		// Reset the current scroll position. 
		yCurrentScroll = yNewPos;


		// Reset the scroll bar. 
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = yCurrentScroll;

		gNoteView->onScroll(yCurrentScroll);
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		if (gIsExistDlg)
		{
			gIsExistDlg = false;
			if (gCurNote == -1)
			{
				onAddNote();
			}
			else
			{
				onEditNote();
			}
			EndDialog(hEditDlg, 0);		
			break;
		}
		if (gCurScene == Scene::VIEW_NOTE)
		{
			RECT rect;
			GetWindowRect(hWnd, &rect);
			gCurNote = gNoteView->onLButtonDown(x, y);
			if (gCurNote >= -1)
			{
				hEditDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_EDIT_NOTE), hWnd, EditNoteProc);
				ShowWindow(hEditDlg, SW_SHOW);
				gRectDlg;
				GetClientRect(hEditDlg, &gRectDlg);
				gRectDlg.left = (rect.left + rect.right - gRectDlg.right + gRectDlg.left)*0.5f;
				gRectDlg.top = (rect.top + rect.bottom - gRectDlg.bottom + gRectDlg.top)*0.5f;
				gRectDlg.right = gRectDlg.right*1.05f;
				gRectDlg.bottom = gRectDlg.bottom*1.18f;
				MoveWindow(hEditDlg, gRectDlg.left, gRectDlg.top, gRectDlg.right, gRectDlg.bottom, SWP_NOZORDER);
				gIsExistDlg = true;
			}
		}
		else
		{

		}
		break;
	}
	case WM_MOVE:
	{

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		gRectDlg.left += x - gPrePos.X;
		gRectDlg.top += y - gPrePos.Y;
		MoveWindow(hEditDlg, gRectDlg.left, gRectDlg.top, gRectDlg.right, gRectDlg.bottom, SWP_NOZORDER);
		gPrePos.X = x;
		gPrePos.Y = y;
		break;
	}
	case WM_NOTIFY:
	{
		auto lia = ((LPNMHDR)lParam);
		switch (lia->code)
		{
		case LVN_ITEMCHANGING:
		case LVN_ITEMCHANGED:
		{
			gCurScene = Scene::VIEW_TAG;
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			if (gIsExistDlg)
			{
				gIsExistDlg = false;
				if (gCurNote == -1)
				{
					onAddNote();
				}
				else
				{
					onEditNote();
				}
				EndDialog(hEditDlg, 0);
				break;
			}
			int itemIndex = ListView_GetNextItem(GetDlgItem(hWnd, ID_TAG_VIEW), -1, LVNI_FOCUSED);
			if (itemIndex == -1)
			{
				break;
			}
			LVITEM selItem;
			selItem.mask = LVIF_PARAM;
			selItem.iItem = itemIndex;
			ListView_GetItem(gTagView->getHWND(), &selItem);

			gNoteView->initNoteList(gPriorityTag->getTagList()[*((int*)selItem.lParam)]->getNoteList());
			RECT rect;
			rect.left = gNoteView->getPosition().X;
			rect.top = gNoteView->getPosition().Y;
			rect.right = gNoteView->getPosition().X + gNoteView->getSizeInView().Width;
			rect.bottom = gNoteView->getPosition().Y + gNoteView->getSizeInView().Height;

			InvalidateRect(hWnd, &rect, FALSE);
			onSetScrollInfo();
			break;
		}
		case NM_CLICK:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			if (gIsExistDlg)
			{
				gIsExistDlg = false;
				if (gCurNote == -1)
				{
					onAddNote();
				}
				else
				{
					onEditNote();
				}
				EndDialog(hEditDlg, 0);
				break;
			}
			break;
		}
		}
		break;
	}
	case WM_DESTROY:
		GdiplusShutdown(gdiplusToken);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
	switch (lpDrawItem->CtlID)
	{
	case ID_BTN_NOTE:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(Color(255, 26, 188, 156));
		graphics->FillRectangle(&brush, 0, 0, 250, 40);

		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18, FontStyleBold, UnitPixel);
		PointF      pointF(0, 9);
		SolidBrush  solidBrush(Color(255, 255, 255, 255));

		graphics->DrawString(L"Ghi chú", -1, &font, pointF, &solidBrush);

		if (graphics)
			delete graphics;
		break;
	}
	case ID_BTN_STATISTIC:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		SolidBrush brush(Color(255, 26, 188, 156));
		graphics->FillRectangle(&brush, 0, 0, 250, 40);

		FontFamily  fontFamily(L"Arial");
		Font        font(&fontFamily, 18, FontStyleBold, UnitPixel);
		PointF      pointF(0, 9);
		SolidBrush  solidBrush(Color(255, 255, 255, 255));

		graphics->DrawString(L"Thống kê", -1, &font, pointF, &solidBrush);

		if (graphics)
			delete graphics;
		break;
	}
	}
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK EditNoteProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
		HWND hStatic = (HWND)lParam;
		HDC hdc = (HDC)wParam;

		//SetTextColor(hdc, RGB(0, 215, 194));
		SetBkMode(hdc, TRANSPARENT);
		//return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
		return (LRESULT)GetStockObject(DC_BRUSH);
	}
	case WM_INITDIALOG:
	{
		if (gCurNote == -1)
		{
			auto hButton = GetDlgItem(hDlg, IDOK);
			SetWindowText(hButton, L"Thêm");
		}
		else
		{
			auto hContent = GetDlgItem(hDlg, IDC_NOTE_CONTENT);
			SetWindowText(hContent, gNoteView->getNote(gCurNote)->getContent().c_str());
			auto hTag = GetDlgItem(hDlg, IDC_TAG);
			SetWindowText(hTag, gNoteView->getNote(gCurNote)->getTagString().c_str());
		}
		
		return (INT_PTR)TRUE;

	}
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDOK:
		{
			gIsExistDlg = false;
			if (gCurNote == -1)
			{
				onAddNote();
			}
			else
			{
				onEditNote();
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		RECT rect;
		GetClientRect(hDlg, &rect);
		auto graphic = new Graphics(hdc);
		SolidBrush brush(Color(255, 255, 255, 255));
		graphic->FillRectangle(&brush, 0, 0, rect.right, rect.bottom);
		//SolidBrush brush2(Color(255, 100, 100, 255));

		//graphic->FillRectangle(&brush2, rect.right*0.1f, rect.bottom*0.76f, rect.right*0.8f, rect.bottom*0.001f);

		delete graphic;
		EndPaint(hDlg, &ps);
		break;
	}
	}
	return (INT_PTR)FALSE;
}


void onAddNote()
{
	WCHAR buffer[10000];
	auto hContent = GetDlgItem(hEditDlg, IDC_NOTE_CONTENT);
	GetWindowText(hContent, buffer, 10000);
	if (buffer[0] == NULL)
	{
		return;
	}

	auto note = new Note();
	note->setWidth(gNoteView->getNoteWidth());
	
	note->setContent(buffer);
	auto hTag = GetDlgItem(hEditDlg, IDC_TAG);
	GetWindowText(hTag, buffer, 10000);
	note->initTagList(buffer);

	gNoteView->addNote(note);
	gPriorityTag->addTag(note);
	gTagView->onAddTag();
	onSetScrollInfo();

	RECT rect;
	rect.left = gNoteView->getPosition().X;
	rect.top = gNoteView->getPosition().Y;
	rect.right = gNoteView->getPosition().X + gNoteView->getSizeInView().Width;
	rect.bottom = gNoteView->getPosition().Y + gNoteView->getSizeInView().Height;
	InvalidateRect(hWnd, &rect, FALSE);
}
void onEditNote()
{
//	WCHAR buffer[10000];
//	auto hContent = GetDlgItem(hEditDlg, IDC_NOTE_CONTENT);
//	GetWindowText(hContent, buffer, 10000);
//	if (buffer[0] == NULL)
//	{
//		return;
//	}
//
//	(*gNoteList)[gCurNote]->setContent(buffer);
//	auto hTag = GetDlgItem(hEditDlg, IDC_TAG);
//	GetWindowText(hTag, buffer, 10000);
//	//(*gNoteList)[gCurNote]->initTagList(buffer);
////	gNoteView->updateNote(gCurNote);
//	//gPriorityTag->addTag(note);
////	gTagView->onAddTag();
//	onSetScrollInfo();
//
//	RECT rect;
//	rect.left = gNoteView->getPosition().X;
//	rect.top = gNoteView->getPosition().Y;
//	rect.right = gNoteView->getPosition().X + gNoteView->getSizeInView().Width;
//	rect.bottom = gNoteView->getPosition().Y + gNoteView->getSizeInView().Height;
//	InvalidateRect(hWnd, &rect, FALSE);
}
void onSetScrollInfo(bool isSetScrollDefault)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;

	if (gNoteView->getActualSize().Height <= gNoteView->getSizeInView().Height || isSetScrollDefault)
	{
		si.nMax = rect.bottom;
		si.nPage = rect.bottom;
		yMaxScroll = si.nMax;
	}
	else
	{
		si.nPage = gNoteView->getSizeInView().Height * 0.85f;
		si.nMax = gNoteView->getActualSize().Height;
		yMaxScroll = si.nMax;
	}
	SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

void LoadNotifyIcon(HWND hwnd)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = ID_NOTIFY_CALLBACK;
	wcscpy(nid.szTip, L"Quick Note");
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	Shell_NotifyIcon(NIM_ADD, &nid);
}

LRESULT CALLBACK HookProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case WM_KEYDOWN:
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		if (p->vkCode == VK_F2)
		{
			if (gIsExistDlg == false)
			{
				gCurNote = -1;
				hEditDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_EDIT_NOTE), hWnd, EditNoteProc);
				ShowWindow(hEditDlg, SW_SHOW);
				RECT rect;
				GetWindowRect(hWnd, &rect);
				GetClientRect(hEditDlg, &gRectDlg);
				gRectDlg.left = (rect.left + rect.right - gRectDlg.right + gRectDlg.left)*0.5f;
				gRectDlg.top = (rect.top + rect.bottom - gRectDlg.bottom + gRectDlg.top)*0.5f;
				gRectDlg.right = gRectDlg.right*1.05f;
				gRectDlg.bottom = gRectDlg.bottom*1.18f;
				MoveWindow(hEditDlg, gRectDlg.left, gRectDlg.top, gRectDlg.right, gRectDlg.bottom, SWP_NOZORDER);
				gIsExistDlg = true;
				SetForegroundWindow(hEditDlg);
				SetActiveWindow(hEditDlg);
			}
		}
	}
	}
	return CallNextHookEx(hHook, message, wParam, lParam);

}