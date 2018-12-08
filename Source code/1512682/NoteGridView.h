#pragma once
#include "Note.h"
#include "AddNote.h"

class NoteGridView
{
private:
	vector<Note*>* _noteList;
	AddNote* _addNoteDlg;
	Point _position;
	Size _actualSize;
	Size _sizeInView;

	Note* _lastNoteColumn[3];
	int _posXDefault[3];
	int _extent;
	int _curScrollY;

	HDC _hdc;
	HDC _hMemDC;
	HWND _hWnd;
private:
	void drawNote(Note* note);
	void setNotePosition(Note* note);
public:
	void setPosition(const Point& pos);
	//void setActualSize(const Size& size);
	void setSizeInView(const Size& size);
	void setHDC(HDC hdc);
	void setHWND(HWND hwnd);

	vector<Note*>* getNoteList();
	Point& getPosition();
	Size& getActualSize();
	Size& getSizeInView();
	HDC& getHDC();
	HWND& getHWND();
	int getNoteWidth();
	Note* getNote(int index);

	void extendHeight(int ex);
	void addNote(Note* note);
	void addNote(const wstring& content, const wstring& tag);
	void initNoteList(vector<Note*>* noteList);
	void updateNote(int index);
	void draw();

	void onScroll(int curScrollY);
	int onLButtonDown(int x, int y);

	NoteGridView();
	~NoteGridView();
};

