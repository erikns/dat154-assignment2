#include "drawing.h"

#include "stdafx.h"

static void draw_ellipse(HDC hdc, point p, size s, COLORREF color) {
	HBRUSH brush = CreateSolidBrush(color);
	HGDIOBJ orig = SelectObject(hdc, brush);
	Ellipse(hdc, p.x, p.y, p.x + s.width, p.y + s.height);
	DeleteObject(brush);
	SelectObject(hdc, orig);
}

void draw_traffic_light(trasim::light_signal *signal, point position, HDC hdc) {
	using namespace trasim;
	size s{ 100, 100 };

	switch (signal->get_state()) {
	case signal_state::GREEN:
		draw_ellipse(hdc, position + point{ 0, 0 }, s, RGB(200, 200, 200));
		draw_ellipse(hdc, position + point{ 0, 100 }, s, RGB(200, 200, 200));
		draw_ellipse(hdc, position + point{ 0, 200 }, s, RGB(0, 255, 0));
		break;
	case signal_state::RED:
		draw_ellipse(hdc, position + point{ 0, 0 }, s, RGB(255, 0, 0));
		draw_ellipse(hdc, position + point{ 0, 100 }, s, RGB(200, 200, 200));
		draw_ellipse(hdc, position + point{ 0, 200 }, s, RGB(200, 200, 200));
		break;
	case signal_state::RED_YELLOW:
		draw_ellipse(hdc, position + point{ 0, 0 }, s, RGB(255, 0, 0));
		draw_ellipse(hdc, position + point{ 0, 100 }, s, RGB(255, 255, 0));
		draw_ellipse(hdc, position + point{ 0, 200 }, s, RGB(200, 200, 200));
		break;
	case signal_state::YELLOW:
		draw_ellipse(hdc, position + point{ 0, 0 }, s, RGB(200, 200, 200));
		draw_ellipse(hdc, position + point{ 0, 100 }, s, RGB(255, 255, 0));
		draw_ellipse(hdc, position + point{ 0, 200 }, s, RGB(200, 200, 200));
		break;
	}
}
