#pragma once

#include <vector>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windows.h>
#include "PriorityTag.h"
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#define M_PI       3.14159265358979323846f
#define MAX_PIE_COUNT 5

class PieChart
{
private:
	int _pieCount;
	PriorityTag* _priorityTag;
	HDC _hdc;
	HDC _hMemDC;
	HDC _hMergingMemDC;
	HBITMAP _background;
	bool _isDrew;
	Point _position;
	int _radius;
	float _percent[MAX_PIE_COUNT];
	float _angle[MAX_PIE_COUNT + 1];
	
	Color _color[MAX_PIE_COUNT];
	RECT _lastRect;
	int _indexPie;
	int _indexChosen;
private:
	float distance(Point a, Point b);
	PointF findPointOfPie(Point center, float deltaAngle);
public:
	static PieChart* create(PriorityTag* priorityTag);
	void setHDC(HDC hdc);
	void setPosition(Point pos);
	void setRadius(int radius);
	void setIndexChosen(int index);
	RECT getBoudingBox();
	Point getPosition();
	int getRadius();
	void update();
	void draw(HWND hwnd);
	bool onMouseMove(Point pos);
	bool onMouseClick(Point pos);
	int getIndexPie();
	int getIndexChosen();
	void clear();
	PieChart();
	~PieChart();
};
