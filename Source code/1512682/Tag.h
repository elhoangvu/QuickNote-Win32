#pragma once
#include <string>
#include <list>
#include "Note.h"
using namespace std;

class Tag
{
private:
	int _count;
	vector<Note*>* _noteList;
public:
	wstring name;
public:
	void onAddTag(Note* note);

	int getCount();
	vector<Note*>* getNoteList();
	Tag();
	~Tag();
};

