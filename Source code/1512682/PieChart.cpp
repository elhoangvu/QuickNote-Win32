#include "stdafx.h"
#include "PieChart.h"

float PieChart::distance(Point a, Point b)
{
	return sqrt((a.X - b.X)*(a.X - b.X) + (a.Y - b.Y)*(a.Y - b.Y));
}

PointF PieChart::findPointOfPie(Point center, float deltaAngle)
{
	float deltaAngle2 = deltaAngle*M_PI / 180.0f;
	float x = _radius*cos(deltaAngle2) + center.X;

	float y;
	float delta = -x*x + 2 * x*center.X - center.X*center.X + _radius*_radius;
	float y1 = center.Y + sqrt(delta);
	float y2 = center.Y - sqrt(delta);
	if (deltaAngle > 180)
	{
		y = y2;
	}
	else
	{
		y = y1;
	}

	auto midPoint = PointF(x, y);
	return midPoint;
}

PieChart * PieChart::create(PriorityTag* priorityTag)
{
	auto pieChart = new (std::nothrow) PieChart;
	pieChart->_priorityTag = priorityTag;

	return pieChart;
}

void PieChart::setHDC(HDC hdc)
{
	_hdc = hdc;
}

void PieChart::setPosition(Point pos)
{
	_position = pos;
}

void PieChart::setRadius(int radius)
{
	_radius = radius;
}

void PieChart::setIndexChosen(int index)
{
	_indexChosen = index;
}

RECT PieChart::getBoudingBox()
{
	RECT rect;
	rect.top = _position.Y - _radius * 0.2f;
	rect.left = _position.X - _radius * 2.8;
	rect.right = rect.left + _radius * 7.8;
	rect.bottom = rect.top + _radius*2.4;
	return rect;
}

Point PieChart::getPosition()
{
	return _position;
}

int PieChart::getRadius()
{
	return _radius;
}


void PieChart::update()
{
	auto tagList = _priorityTag->getTagList();
	if (tagList.size() <= MAX_PIE_COUNT)
	{
		_pieCount = tagList.size();
	}
	else
	{
		_pieCount = MAX_PIE_COUNT;
	}

	int sum = 0;


	for (int i = 0; i < _pieCount; i++)
	{
		sum += tagList[i]->getCount();
	}

	if (tagList.size() == 0)
	{
		return;
	}

	for (int i = 0; i < _pieCount; i++)
	{
		_percent[i] = ((double)tagList[i]->getCount() / (double)sum) * 100;
		_angle[i + 1] = _percent[i] * 3.6 + _angle[i];
	}
}

void PieChart::draw(HWND hwnd)
{
	if (_hdc == NULL)
	{
		return;
	}

	if (!_isDrew)	// Init Memory DC
	{
		GetClientRect(hwnd, &_lastRect);
		_hMemDC = CreateCompatibleDC(_hdc);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);

		(HBITMAP)SelectObject(_hMemDC, _background);

		_hMergingMemDC = CreateCompatibleDC(_hdc);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);

		(HBITMAP)SelectObject(_hMergingMemDC, _background);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);

		_isDrew = true;
	}
	if (_isDrew)	// Reset Memory DC
	{
		FillRect(_hMergingMemDC, &_lastRect, HBRUSH(COLOR_BTNFACE + 1));
		FillRect(_hMemDC, &_lastRect, HBRUSH(COLOR_BTNFACE + 1));
	}
	if (_priorityTag->getTagList().size() == 0)
	{
		return;
	}
	this->update();

	// Graphics MemDC
	auto graphics = new Graphics(_hMemDC);
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// Fill Pie
	Rect ellipseRect(_position.X, _position.Y, _radius * 2, _radius * 2);
	for (int i = 0; i < _pieCount; i++)
	{
		SolidBrush brush(_color[i]);
		graphics->FillPie(&brush, ellipseRect, _angle[i], _angle[i + 1] - _angle[i] + 0.5f);
	}

	// Fill center Circle
	SolidBrush brush3(Color(255, 54, 60, 58));
	Rect ellipseRect3(_position.X + _radius*0.55f, _position.Y + _radius*0.55f, _radius*0.9f, _radius*0.9f);
	graphics->FillEllipse(&brush3, ellipseRect3);

	// Transfer MemDC to Merging MemDC
	if (!BitBlt(_hMergingMemDC, ellipseRect.X - _radius * 0.2f, ellipseRect.Y - _radius * 0.16f, ellipseRect.Width + _radius * 0.32f, ellipseRect.Height + _radius * 0.32f, _hMemDC, ellipseRect.X - _radius * 0.16f, ellipseRect.Y - _radius * 0.16f, SRCCOPY))
	{
		MessageBox(hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}

	// Graphics Merging MemDC
	auto graphics2 = new Graphics(_hMergingMemDC);
	graphics2->SetSmoothingMode(SmoothingModeAntiAlias);
	
	WCHAR bufString[33];
	float delta = 0;
	
	auto midPos = Point(_position.X + _radius, _position.Y + _radius);

	std::vector<PointF> leftPos;		// Mid pos of left pies
	std::vector<PointF> rightPos;	// Mid pos of right pies
	int begin = 0;
	bool isHalfCir = false;

	// Calc mid pos of pies in PieChart
	for (int i = 0; i < _pieCount; i++)
	{
		SolidBrush brush3(_color[i]);

// Find mid pos
auto pos = findPointOfPie(midPos, (_angle[i + 1] + _angle[i]) / 2.0f);

// Draw point in mid pie
RectF ellipseRect2(pos.X - 7, pos.Y - 4, 8, 8);
graphics2->FillEllipse(&brush3, ellipseRect2.X, ellipseRect2.Y, ellipseRect2.Width, ellipseRect2.Height);

// push pos
if (pos.X < midPos.X)
{
	leftPos.push_back(pos);
	isHalfCir = true;
}
else
{
	if (i == 0)
	{
		rightPos.push_back(pos);
		continue;
	}
	if (isHalfCir)
	{
		rightPos.insert(rightPos.begin() + begin++, pos);
	}
	else
	{
		rightPos.push_back(pos);
	}
}
	}

	int leftCount = leftPos.size();
	int rightCount = rightPos.size();

	int leftDelta = _radius * 2 / (leftCount + 1);
	int rightDelta = _radius * 2 / (rightCount + 1);

	int bottom = _position.Y + _radius * 2;

	int downCount = 0;
	int upCount = 0;

	delta = 0;

	// Draw description of left pie
	for (int i = 0; i < leftCount; i++)
	{
		auto pos1 = PointF(_position.X - _radius*0.5f, leftPos[i].Y);
		auto pos2 = PointF(pos1.X, bottom - leftDelta*(i + 1));

		int upDownCount = 0;
		if (leftPos[i].Y < pos2.Y)
		{
			pos1 = PointF(pos1.X - _radius*0.15f*upCount, pos1.Y);
			upDownCount = ++upCount;
		}
		else
		{
			pos1 = PointF(pos1.X + _radius*0.15f*downCount, pos1.Y);
			upDownCount = ++downCount;
		}
		auto pos3 = PointF(pos1.X, pos2.Y);
		int index = i + rightCount - begin;
		SolidBrush brush3(_color[index]);

		if (leftCount > 2 && (i == leftCount - 1 || i == 0))
		{
			delta = _radius*0.25;
		}
		else
		{
			delta = 0;
		}
		Pen pen(_color[index], 2);
		auto pos4 = PointF(_position.X - _radius - delta, pos3.Y);
		graphics2->DrawLine(&pen, pos1, leftPos[i]);
		graphics2->DrawLine(&pen, pos1, pos3);
		graphics2->DrawLine(&pen, pos3, pos4);
		graphics2->FillRectangle(&brush3, pos4.X - _radius*0.5f, pos4.Y - _radius*0.15f, _radius*0.5f, _radius*0.3f);

		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		FontFamily  fontFamily(L"Arial");
		float size = _radius / 6;
		if (wcslen(bufString) == _pieCount)
		{
			size = _radius / 7;
		}
		Font        font(&fontFamily, size, FontStyleBold, UnitPixel);
		float posXString = _radius*0.5;
		if (wcslen(bufString) == 4)
		{
			posXString = _radius*0.45f;
		}
		PointF      pointF(pos4.X - posXString, pos4.Y - _radius*0.1f);
		SolidBrush  solidBrush(Color(255, 255, 255, 255));

		graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);
		float dt = 0;

		wstring name = _priorityTag->getTagList()[index]->name;
		if (name.size() > 10)
		{
			name.erase(10, name.size());
			name.push_back(L'.');
			name.push_back(L'.');
			name.push_back(L'.');
		}
		int len = name.size();
		if (len > 3)
		{
			dt = (len - 5)*_radius*0.04f;
		}
		else
		{
			dt = -_radius*0.2f;
		}
		pointF = PointF(pos4.X - posXString - _radius*0.5f - dt, pos4.Y - _radius*0.1f);

		SolidBrush  solidBrush2(_color[index]);
		Font        font2(&fontFamily, _radius / 7, FontStyleBold, UnitPixel);
		graphics2->DrawString(name.c_str(), -1, &font2, pointF, &solidBrush2);
	}

	downCount = 0;
	upCount = 0;

	// Draw description of left pie
	for (int i = rightCount - 1; i >= 0; i--)
	{
		auto pos1 = PointF(_position.X + _radius*2.5f, rightPos[i].Y);
		auto pos2 = PointF(pos1.X, bottom - rightDelta*(rightCount - i));
		int upDownCount = 0;
		if (rightPos[i].Y < pos2.Y)
		{
			pos1 = PointF(pos1.X + _radius*0.15f*upCount, pos1.Y);
			upDownCount = ++upCount;
		}
		else
		{
			pos1 = PointF(pos1.X - _radius*0.15f*downCount, pos1.Y);
			upDownCount = ++downCount;
		}

		auto pos3 = PointF(pos1.X, pos2.Y);
		int index = _pieCount + i - begin;
		if (i >= begin)
		{
			index = i - begin;
		}
		SolidBrush brush3(_color[index]);
		//auto pos4 = Point(pos3.X + _radius*0.2f*(i + 1), pos3.Y);
		if (rightCount > 2 && (i == rightCount - 1 || i == 0))
		{
			delta = _radius*0.25;
		}
		else
		{
			delta = 0;
		}
		Pen pen(_color[index], 2);
		auto pos4 = PointF(_position.X + _radius*3.0f + delta, pos3.Y);
		graphics2->DrawLine(&pen, pos1, rightPos[i]);
		graphics2->DrawLine(&pen, pos1, pos3);
		graphics2->DrawLine(&pen, pos3, pos4);
		graphics2->FillRectangle(&brush3, (REAL)pos4.X, pos4.Y - _radius*0.15f, _radius*0.5f, _radius*0.3f);

		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		FontFamily  fontFamily(L"Arial");
		float size = _radius / 6;
		if (wcslen(bufString) == _pieCount)
		{
			size = _radius / 7;
		}
		Font        font(&fontFamily, size, FontStyleBold, UnitPixel);
		float posXString = 0;
		if (wcslen(bufString) == 4)
		{
			posXString = _radius*0.05f;
		}
		PointF      pointF(pos4.X + posXString, pos4.Y - _radius*0.1f);
		SolidBrush  solidBrush(Color(255, 255, 255, 255));

		graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);

		pointF = PointF(pos4.X + posXString + _radius*0.62f, pos4.Y - _radius*0.1f);
		SolidBrush  solidBrush2(_color[index]);
		Font        font2(&fontFamily, _radius / 7, FontStyleBold, UnitPixel);
		wstring name = _priorityTag->getTagList()[index]->name;
		if (name.size() > 10)
		{
			name.erase(10, name.size());
			name.push_back(L'.');
			name.push_back(L'.');
			name.push_back(L'.');
		}
		graphics2->DrawString(name.c_str(), -1, &font2, pointF, &solidBrush2);
	}

	// When mouse moves or clicks inside the pies
	if (_indexPie != -1 || _indexChosen != -1)
	{
		int index = _indexChosen == -1 ? _indexPie : _indexChosen;
		Rect ellipseRect3(_position.X - _radius * 0.15f, _position.Y - _radius * 0.15f, _radius*2.3f, _radius*2.3f);
		SolidBrush brush(_color[index]);
		graphics2->FillPie(&brush, ellipseRect3, _angle[index] - 2, _angle[index + 1] - _angle[index] + 4);

	}

	// Draw center circle again in Mering MemDC
	SolidBrush brush2(Color(255, 54, 60, 58));
	Rect ellipseRect2(_position.X + _radius / 2.0f, _position.Y + _radius / 2.0f, _radius, _radius);
	graphics2->FillEllipse(&brush2, ellipseRect2);
	

	// Draw String inside the certer circle
	// Init properties of DrawString Func
	FontFamily  fontFamily(L"Arial");
	Font        font(&fontFamily, _radius / 6, FontStyleBold, UnitPixel);
	PointF      pointF;
	SolidBrush  solidBrush(Color(255, 255, 255, 255));
	Font        font2(&fontFamily, _radius / 8, FontStyleBold, UnitPixel);
	int index = -1;

	if (_indexPie == -1 && _indexChosen == -1)	// On mouse leave
	{

		Font        font0(&fontFamily, 10, FontStyleBold, UnitPixel);
		SolidBrush  solidBrush2(Color(255, 255, 255, 255));
		StringFormat format;
		format.SetAlignment(StringAlignment::StringAlignmentCenter);
		graphics2->DrawString(L"Các thẻ sử dụng nhiều nhất", -1, &font0, pointF, &solidBrush);
		RectF layoutRect(_position.X + _radius*0.75f, _position.Y +_radius*0.75f, _radius*0.5f, _radius*0.5f);
		graphics2->DrawString(L"Các thẻ sử dụng\r\nnhiều nhất", -1, &font0, layoutRect, &format, &solidBrush2);
	}	
	else	// On mouse move
	{
		index = _indexPie == -1 ? _indexChosen : _indexPie;

		delta = 0;
		if (_percent[index] < 10)
		{
			delta = _radius*0.055f;
		}
		pointF = PointF(_position.X + _radius*0.84f + delta, _position.Y + _radius*1.1f);
		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		graphics2->DrawString(bufString, -1, &font2, pointF, &solidBrush);
	
		delta = 0;
		auto len = _priorityTag->getTagList()[index]->name.size();
		delta = len*_radius*0.027;

		pointF = PointF(_position.X + _radius - delta, _position.Y + _radius*0.75f);
		graphics2->DrawString(_priorityTag->getTagList()[index]->name.c_str(), -1, &font2, pointF, &solidBrush);
	}
	if (index != -1)
	{
		wsprintf(bufString, L"%d Tag", _priorityTag->getTagList()[index]->name.size());

		auto len = wcslen(bufString);
		delta = 0;
		if (len > 4)
		{
			delta = (len - 4) *_radius*0.045f;
		}
		pointF = PointF(_position.X + _radius*0.83f - delta, _position.Y + _radius*0.9f);
		graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);
	}
	// Transfer Mering MemDC to HDC
	if (!BitBlt(_hdc, ellipseRect.X - _radius*2.8, ellipseRect.Y - _radius * 0.2f, _radius *7.8, _radius*2.4, _hMergingMemDC, ellipseRect.X - _radius * 2.8, ellipseRect.Y - _radius * 0.2f, SRCCOPY))
	{
		MessageBox(hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}

	// Release graphics
	if (graphics2)
		delete graphics2;
	if (graphics)
		delete graphics;
}

bool PieChart::onMouseMove(Point pos)
{
	COLORREF color = GetPixel(_hMemDC, pos.X, pos.Y);
	for (int i = 0; i < _pieCount; i++)
	{
		if (color == _color[i].ToCOLORREF())
		{
			_indexPie = i;
			return true;
		}
	}
	_indexPie = -1;
	return false;
}

bool PieChart::onMouseClick(Point pos)
{
	COLORREF color = GetPixel(_hMemDC, pos.X, pos.Y);
	for (int i = 0; i < MAX_PIE_COUNT; i++)
	{
		if (color == _color[i].ToCOLORREF())
		{
			_indexChosen = i;
			return true;
		}
	}
	_indexChosen = -1;
	return false;
}

int PieChart::getIndexPie()
{
	return _indexPie;
}

int PieChart::getIndexChosen()
{
	return _indexChosen;
}

void PieChart::clear()
{
	_indexPie = -1;
	_indexChosen = -1;
	_angle[0] = 0;
	for (int i = 0; i < _pieCount; i++)
	{
		_percent[i] = 100.0f / (float)_pieCount;
		_angle[i + 1] = 360;
	}
}

PieChart::PieChart()
{
	_hdc = NULL;
	_radius = 100;
	_isDrew = false;
	_indexPie = -1;
	_indexChosen = -1;
	_position = Point(0, 0);
	_color[0] = Color(255, 238, 53, 91);
	_color[1] = Color(255, 217, 191, 173);
	_color[2] = Color(255, 100, 201, 213);
	_color[3] = Color(255, 50, 169, 146);
	_color[4] = Color(255, 238, 167, 58);
	_color[5] = Color(255, 156, 107, 179);
	_angle[0] = 0;
	for (int i = 0; i < MAX_PIE_COUNT; i++)
	{
		_percent[i] = 100.0f / 6.0f;
		_angle[i + 1] = 360;
	}
}


PieChart::~PieChart()
{
}
