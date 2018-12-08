#include "stdafx.h"
#include "Note.h"


void Note::setSize(const Size & size)
{
	_size = size;
}

void Note::setPosition(const Point & pos)
{
	_position = pos;
}

void Note::setIndex(int index)
{
	_index = index;
}

void Note::setContent(wstring content)
{
	_content = content;

	auto graphic = new Graphics(GetDC(GetActiveWindow()));
	int width = _size.Width;
	int extent = width*0.05;
	RectF layoutRect(0, 0, width - extent, 1000);
	RectF boundRect;
	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	if (getContent().size() > 200)
	{
		wstring contentInView;
		contentInView = getContent().substr(0, 200);
		contentInView += L"...";
		graphic->MeasureString(contentInView.c_str(), -1, &font, layoutRect, &boundRect);
	}
	else
	{
		graphic->MeasureString(getContent().c_str(), -1, &font, layoutRect, &boundRect);
	}

	_size.Height += boundRect.Height + extent;

	delete graphic;
}

void Note::setWidth(int width)
{
	_size.Width = width;
}

void Note::setHeight(int height)
{
	_size.Height = height;
}

void Note::setColumn(int column)
{
	_column = column;
}

void Note::addTag(const wstring& tag)
{
	_tagList.push_back(tag);
}

void Note::initTagList(wstring tagList)
{
	int i = 0;
	_tagList.clear();
	while (i < tagList.size())
	{
		wstring tag;
		for (; i < tagList.size() && tagList[i] == L' '; i++) {}
		for (; i < tagList.size() && tagList[i] != L','; i++)
		{
			tag.push_back(tagList[i]);
		}
		if (tag.size() > 0)
		{
			int j;
			for (j = tag.size() - 1; j >= 0 && tag[j]==L' '; j--) {}
			if (j >= 0)
			{
				bool isExistTag = false;
				tag.erase(j + 1, tag.size());
				for (auto iTag : _tagList)
				{
					if (iTag == tag)
					{
						isExistTag = true;
						break;
					}
				}
				if (!isExistTag)
				{
					_tagList.push_back(tag);
				}
			}	
		}
		i++;
	}

	auto graphic = new Graphics(GetDC(GetActiveWindow()));
	int width = _size.Width;
	int extent = width*0.05;
	RectF layoutRect(0, 0, width - extent, 1000);
	RectF boundRect;
	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	auto tagString = getTagString();
	if (tagString.size() > 16)
	{
		wstring tagInView;
		tagInView = tagString.substr(0, 16);
		tagInView += L"...";
		graphic->MeasureString((L"Tag: " + tagInView).c_str(), -1, &font, layoutRect, &boundRect);
	}
	else
	{
		graphic->MeasureString((L"Tag: " + tagString).c_str(), -1, &font, layoutRect, &boundRect);
	}

	//graphic->MeasureString((L"Tag: " + getTagString()).c_str(), -1, &font, layoutRect, &boundRect);

	_size.Height += boundRect.Height + extent * 2;

	delete graphic;
}

wstring & Note::getContent()
{
	return _content;
}

list<wstring>& Note::getTagList()
{
	return _tagList;
}

wstring Note::getTagString()
{
	wstring s;
	for (auto tag : _tagList)
	{
		s += tag + L", ";
	}
	if (s.size() >= 2)
	{
		s.pop_back();
		s.pop_back();
	}
	return s;
}

int Note::getIndex()
{
	return _index;
}

int Note::getColumn()
{
	return _column;
}

Point & Note::getPosition()
{
	return _position;
}

Size & Note::getSize()
{
	return _size;
}

void Note::draw(HDC hdc, int offset)
{
	auto graphic = new Graphics(hdc);
	graphic->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	SolidBrush brush(Color(255, 255, 255));
	SolidBrush brushShadow(Color(20, 0, 0, 0));
	graphic->FillRectangle(&brushShadow, _position.X + 3, _position.Y - offset + 3, _size.Width, _size.Height);
	graphic->FillRectangle(&brush, _position.X, _position.Y - offset, _size.Width, _size.Height);


	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0, 0);
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 0, 0, 0));
	StringFormat format;
	format.SetAlignment(StringAlignment::StringAlignmentNear);

	int extent = _size.Width*0.05;
	RectF layoutRect(_position.X + extent, _position.Y + extent - offset, _size.Width - extent, _size.Height - extent);
	RectF boundRect;
	// Measure the string.
	//graphic->MeasureString(_content.c_str(), -1, &font, layoutRect, &boundRect);
	
	if (_content.size() > 200)
	{
		wstring contentInView;
		contentInView = _content.substr(0, 200);
		contentInView += L"...";
		graphic->DrawString(contentInView.c_str(), -1, &font, layoutRect, &format, &solidBrush);
		graphic->MeasureString(contentInView.c_str(), -1, &font, layoutRect, &boundRect);
	}
	else
	{
		graphic->DrawString(_content.c_str(), -1, &font, layoutRect, &format, &solidBrush);
		graphic->MeasureString(_content.c_str(), -1, &font, layoutRect, &boundRect);
	}
	
	layoutRect.Y += boundRect.Height + extent;

	// Draw tag
	auto tagString = getTagString();
	if (tagString.size() > 16)
	{
		wstring tagInView;
		tagInView = tagString.substr(0, 16);
		tagInView += L"...";
		graphic->DrawString((L"Tag: " + tagInView).c_str(), -1, &font, layoutRect, &format, &solidBrush);
		graphic->MeasureString((L"Tag: " + tagInView).c_str(), -1, &font, layoutRect, &boundRect);
	}
	else
	{
		graphic->DrawString((L"Tag: " + tagString).c_str(), -1, &font, layoutRect, &format, &solidBrush);
		graphic->MeasureString((L"Tag: " + tagString).c_str(), -1, &font, layoutRect, &boundRect);
	}

	//graphic->DrawString((L"Tag: " + getTagString()).c_str(), -1, &font, layoutRect, &format, &solidBrush);
	// Draw a rectangle that represents the size of the string.
	//graphic->DrawRectangle(&Pen(Color(255, 0, 0, 0)), boundRect);

	delete graphic;
}

Note::Note()
{
	_index = 0;
}

Note::~Note()
{
}
