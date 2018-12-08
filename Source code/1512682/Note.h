#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <list>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
using namespace std;

class Note
{
private:
	wstring _content;
	list<wstring> _tagList;
	int _index;
	int _column;
protected:
	Point _position;
	Size _size;
public:
	virtual void setSize(const Size& size);
	virtual void setPosition(const Point& pos);
	void setIndex(int index);
	void setContent(wstring content);
	void setWidth(int width);
	void setHeight(int height);
	void setColumn(int column);

	void addTag(const wstring& tag);
	void initTagList(wstring tagList);

	wstring& getContent();
	list<wstring>& getTagList();
	wstring getTagString();
	int getIndex();
	int getColumn();
	virtual Point& getPosition();
	virtual Size& getSize();

	virtual void draw(HDC hdc, int offset);

	Note();
	~Note();
};

