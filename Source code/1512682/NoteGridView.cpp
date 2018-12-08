#include "stdafx.h"
#include "NoteGridView.h"
#include "resource.h"

NoteGridView::NoteGridView()
{
	_noteList = new vector<Note*>;
	_addNoteDlg = new AddNote();
	_lastNoteColumn[0] = NULL;
	_lastNoteColumn[1] = NULL;
	_lastNoteColumn[2] = NULL;
	_curScrollY = 0;
}


NoteGridView::~NoteGridView()
{
	if (_noteList)
	{
		delete _noteList;
	}
	ReleaseDC(_hWnd, _hMemDC);
}

void NoteGridView::drawNote(Note * note)
{
	int top;
	int bottom;
	top = _curScrollY;
	bottom = _curScrollY + _sizeInView.Height;
	int x = note->getPosition().Y;
	if (note->getPosition().Y < top)
	{
		if (note->getPosition().Y + note->getSize().Height > top)
		{
			note->draw(_hMemDC, _curScrollY);
		}
	}
	else if (note->getPosition().Y >= top && note->getPosition().Y < bottom)
	{
		note->draw(_hMemDC, _curScrollY);
	}
}

void NoteGridView::setNotePosition(Note * note)
{
	int index = note->getIndex();
	int posY;
	int noteHeight;
	if (index < 3)
	{
		posY = _extent + 100;
	}
	else
	{
		if (_lastNoteColumn[0]->getPosition().Y + _lastNoteColumn[0]->getSize().Height
			<= _lastNoteColumn[1]->getPosition().Y + _lastNoteColumn[1]->getSize().Height
			&& _lastNoteColumn[0]->getPosition().Y + _lastNoteColumn[0]->getSize().Height
			<= _lastNoteColumn[2]->getPosition().Y + _lastNoteColumn[2]->getSize().Height)
		{
			posY = _lastNoteColumn[0]->getPosition().Y + _lastNoteColumn[0]->getSize().Height + _extent;
			index = 0;
		}
		else if (_lastNoteColumn[1]->getPosition().Y + _lastNoteColumn[1]->getSize().Height
			<= _lastNoteColumn[2]->getPosition().Y + _lastNoteColumn[2]->getSize().Height
			&& _lastNoteColumn[1]->getPosition().Y + _lastNoteColumn[1]->getSize().Height
			<= _lastNoteColumn[0]->getPosition().Y + _lastNoteColumn[0]->getSize().Height)
		{
			posY = _lastNoteColumn[1]->getPosition().Y + _lastNoteColumn[1]->getSize().Height + _extent;
			index = 1;
		}
		else
		{
			posY = _lastNoteColumn[2]->getPosition().Y + _lastNoteColumn[2]->getSize().Height + _extent;
			index = 2;
		}

	}

	///
	note->setColumn(index);
	note->setPosition(Point(_posXDefault[index], posY));
	_lastNoteColumn[index] = note;
	///
	noteHeight = note->getPosition().Y + note->getSize().Height;

	if (noteHeight > _actualSize.Height)
	{
		_actualSize.Height = noteHeight;
	}
}

void NoteGridView::setPosition(const Point & pos)
{
	_position = pos;
}

//void NoteGridView::setActualSize(const Size & size)
//{
//	_actualSize = size;
//
//}

void NoteGridView::setSizeInView(const Size & size)
{
	_sizeInView = size;
	_extent = _sizeInView.Width*0.025f;
	_posXDefault[0] = _extent;
	_posXDefault[1] = _sizeInView.Width*0.35f;
	_posXDefault[2] = _sizeInView.Width*0.675f;
	_actualSize = Size(_sizeInView.Width, 0);
	_addNoteDlg->setPosition(Point(_sizeInView.Width*0.05f, _sizeInView.Height*0.05f));
	_addNoteDlg->setSize(Size(_sizeInView.Width*0.9f, _sizeInView.Height*0.075f));
}

void NoteGridView::setHDC(HDC hdc)
{
	_hdc = hdc;
}

void NoteGridView::setHWND(HWND hwnd)
{
	_hWnd = hwnd;
}

vector<Note*>* NoteGridView::getNoteList()
{
	return _noteList;
}

Point & NoteGridView::getPosition()
{
	return _position;
}

Size & NoteGridView::getActualSize()
{
	return _actualSize;
}

Size & NoteGridView::getSizeInView()
{
	return _sizeInView;
}

HDC & NoteGridView::getHDC()
{
	return _hdc;
}

HWND & NoteGridView::getHWND()
{
	return _hWnd;
}

int NoteGridView::getNoteWidth()
{
	return _posXDefault[1] - _posXDefault[0] - _extent;
}

Note * NoteGridView::getNote(int index)
{
	return (*_noteList)[index];
}

void NoteGridView::extendHeight(int ex)
{
	_actualSize.Height += ex;
}

void NoteGridView::addNote(Note * note)
{
	int index = _noteList->size();
	note->setIndex(index);

	//setNotePosition(note);
	_noteList->insert(_noteList->begin(), note);
	updateNote(0);
	//_noteList->push_back(note);

}


//
//void NoteGridView::addNote(Note * note)
//{
//	int index = _noteList->size();
//	int posY;
//
//	note->setIndex(index);
//	///
//	if (index < 3)
//	{
//		posY = _extent + 100;
//	}
//	else
//	{
//		posY = (*_noteList)[index - 3]->getPosition().Y + (*_noteList)[index - 3]->getSize().Height + _extent;
//	}
//
//	///
//	note->setPosition(Point(_posXDefault[index % 3], posY));
//
//	///
//	int noteHeight = note->getPosition().Y + note->getSize().Height;
//	if (noteHeight > _actualSize.Height)
//	{
//		_actualSize.Height = noteHeight;
//	}
//	_noteList->push_back(note);
//}

void NoteGridView::addNote(const wstring & content, const wstring & tag)
{
	auto noteTemp = new Note();
	noteTemp->setContent(content);
	noteTemp->initTagList(tag);
	addNote(noteTemp);

}

void NoteGridView::initNoteList(vector<Note*>* noteList)
{
	_noteList = noteList;
	if (noteList == NULL)
	{
		return;
	}
	updateNote(0);
}

void NoteGridView::updateNote(int index)
{
	if (index < 3)
	{
		int i;
		_lastNoteColumn[0] = NULL;
		_lastNoteColumn[1] = NULL;
		_lastNoteColumn[2] = NULL;
		_actualSize.Height = 0;
		for (i = 0; i < index; i++)
		{
			_lastNoteColumn[i] = (*_noteList)[i];

			int noteHeight = (*_noteList)[i]->getPosition().Y + (*_noteList)[i]->getSize().Height;

			if (noteHeight > _actualSize.Height)
			{
				_actualSize.Height = noteHeight;
			}
		}
	}
	else
	{
		bool col[3] = { false };
		_actualSize.Height = 0;
		for (int i = index - 1; i >= 0; i--)
		{
			int iCol = (*_noteList)[i]->getColumn();
			if (!col[iCol])
			{
				_lastNoteColumn[iCol] = (*_noteList)[i];
				int noteHeight = (*_noteList)[i]->getPosition().Y + (*_noteList)[i]->getSize().Height;

				if (noteHeight > _actualSize.Height)
				{
					_actualSize.Height = noteHeight;
				}
				col[iCol] = true;
			}
			if (col[0] && col[1] && col[2])
			{
				break;
			}
		}
	}
	int i = 0;
	for (auto note : *_noteList)
	{
		note->setIndex(i);
		note->setWidth(getNoteWidth());
		setNotePosition(note);
		i++;
	}
}

void NoteGridView::draw()
{
	static bool isDrew = false;
	RECT rect;
	if (!isDrew)
	{
		GetClientRect(_hWnd, &rect);
		_hMemDC = CreateCompatibleDC(_hdc);
		auto background = CreateCompatibleBitmap(_hdc, _sizeInView.Width, _sizeInView.Height);
		FillRect(_hMemDC, &rect, (HBRUSH)(COLOR_BTNFACE + 1));
		(HBITMAP)SelectObject(_hMemDC, background);

		background = CreateCompatibleBitmap(_hdc, _sizeInView.Width, _sizeInView.Height);
		isDrew = true;
	}
	rect.left = 0;
	rect.top = 0;
	rect.right = _sizeInView.Width;
	rect.bottom = _sizeInView.Height;
	FillRect(_hMemDC, &rect, (HBRUSH)(COLOR_BTNFACE + 1));

	drawNote(_addNoteDlg);
	for (auto note : *_noteList)
	{
		drawNote(note);
	}
	if (!BitBlt(_hdc, _position.X, _position.Y, _sizeInView.Width, _sizeInView.Height, _hMemDC, 0, 0, SRCCOPY))
	{
		MessageBox(_hWnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}
}

void NoteGridView::onScroll(int curScrollY)
{
	_curScrollY = curScrollY;
}

int NoteGridView::onLButtonDown(int x, int y)
{
	if (x > _position.X && x<_position.X + _sizeInView.Width
		&&y>_position.Y && y < _position.Y + _sizeInView.Height)
	{
		if (x > _position.X + _addNoteDlg->getPosition().X 
			&& x < _position.X + _addNoteDlg->getPosition().X + _addNoteDlg->getSize().Width
			&&y > _position.Y + _addNoteDlg->getPosition().Y - _curScrollY 
			&& y < _position.Y + _addNoteDlg->getPosition().Y + _addNoteDlg->getSize().Height - _curScrollY)
		{
			return -1;
		}
		for (auto tag : (*_noteList))
		{
			if (x > _position.X + tag->getPosition().X && x < _position.X + tag->getPosition().X + tag->getSize().Width
				&&y > _position.Y + tag->getPosition().Y - _curScrollY && y < _position.Y + tag->getPosition().Y + tag->getSize().Height - _curScrollY)
			{
				return tag->getIndex();
			}
		}
	}
	return -2;
}
