#include "stdafx.h"
#include "AddNote.h"

void AddNote::draw(HDC hdc, int offset)
{
	auto graphic = new Graphics(hdc);
	graphic->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	SolidBrush brush(Color(255, 255, 255));
	SolidBrush brushShadow(Color(20, 0, 0, 0));
	graphic->FillRectangle(&brushShadow, _position.X + 3, _position.Y - offset + 3, _size.Width, _size.Height);
	graphic->FillRectangle(&brush, _position.X, _position.Y - offset, _size.Width, _size.Height);


	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(_position.X + _size.Height*0.3f, _position.Y + _size.Height*0.3f - offset);
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 0, 0, 0));
	StringFormat format;
	format.SetAlignment(StringAlignment::StringAlignmentNear);

	int extent = _size.Height*0.75f;
	
	graphic->DrawString(L"Tạo ghi chú...", -1, &font, pointF, &solidBrush);

	delete graphic;
}

AddNote::AddNote()
{
}

AddNote::~AddNote()
{
}
