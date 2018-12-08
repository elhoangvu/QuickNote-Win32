#include "stdafx.h"
#include "Tag.h"


void Tag::onAddTag(Note* note)
{
	_count++;
	for (auto iNote : *_noteList)
	{
		if (iNote == note)
		{
			return;
		}
	}
	(*_noteList).insert((*_noteList).begin(), note);
}

int Tag::getCount()
{
	return _count;
}

vector<Note*>* Tag::getNoteList()
{
	return _noteList;
}

Tag::Tag()
{
	_noteList = new vector<Note*>;
	_count = 0;
}


Tag::~Tag()
{
	if (_noteList)
	{
		delete _noteList;
	}
}
