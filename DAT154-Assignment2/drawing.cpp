#include "drawing.h"

#include "stdafx.h"

static const auto RED = RGB(255, 0, 0);
static const auto GRAY = RGB(200, 200, 200);
static const auto GREEN = RGB(0, 255, 0);
static const auto YELLOW = RGB(255, 255, 0);

static void draw_ellipse(HDC hdc, point p, size s, COLORREF color) {
	HBRUSH brush = CreateSolidBrush(color);
	HGDIOBJ orig = SelectObject(hdc, brush);
	Ellipse(hdc, p.x, p.y, p.x + s.x, p.y + s.y);
	DeleteObject(brush);
	SelectObject(hdc, orig);
}

void draw_traffic_light(trasim::light_signal *signal, point position, float scale, HDC hdc) {
	using namespace trasim;
	size s{ 100, 100 };
	s = s * scale;

	switch (signal->get_state()) {
	case signal_state::GREEN:
		draw_ellipse(hdc, position + (point{ 0, 0 } * scale), s, RGB(200, 200, 200));
		draw_ellipse(hdc, position + (point{ 0, 100 } * scale), s, GRAY);
		draw_ellipse(hdc, position + (point{ 0, 200 } * scale), s, GREEN);
		break;
	case signal_state::RED:
		draw_ellipse(hdc, position + (point{ 0, 0 } * scale), s, RED);
		draw_ellipse(hdc, position + (point{ 0, 100 } * scale), s, GRAY);
		draw_ellipse(hdc, position + (point{ 0, 200 } * scale), s, GRAY);
		break;
	case signal_state::RED_YELLOW:
		draw_ellipse(hdc, position + (point{ 0, 0 } * scale), s, RED);
		draw_ellipse(hdc, position + (point{ 0, 100 } * scale), s, YELLOW);
		draw_ellipse(hdc, position + (point{ 0, 200 } * scale), s, GRAY);
		break;
	case signal_state::YELLOW:
		draw_ellipse(hdc, position + (point{ 0, 0 } * scale), s, GRAY);
		draw_ellipse(hdc, position + (point{ 0, 100 } * scale), s, YELLOW);
		draw_ellipse(hdc, position + (point{ 0, 200 } * scale), s, GRAY);
		break;
	}
}
