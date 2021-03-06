#include "drawing.h"

#include "stdafx.h"

static const auto RED = RGB(255, 0, 0);
static const auto GRAY = RGB(200, 200, 200);
static const auto GREEN = RGB(0, 255, 0);
static const auto YELLOW = RGB(255, 255, 0);

static void draw_ellipse(HDC hdc, point p, size s, COLORREF color) {
	drawing_context ctx{ hdc };
	ctx.use_brush(color);
	ctx.ellipse(p, s);
}

void draw_traffic_light(const trasim::light_signal *signal, point position, float scale, HDC hdc) {
	using namespace trasim;
	size s{ 100, 100 };
	s = s * scale;

	switch (signal->get_state()) {
	case signal_state::GREEN:
		draw_ellipse(hdc, position + (point{ 0, 0 } * scale), s, GRAY);
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

void draw_cars(const std::vector<trasim::car> &cars, HDC hdc) {
	drawing_context dc{ hdc };
	dc.use_brush(RGB(255, 100, 100));

	for (auto c : cars) {
		auto pos_vec = c.position();
		point pos{ pos_vec.x, pos_vec.y };
		dc.ellipse(pos, size{ 10, 10 });
	}
}

void draw_signal_lines(int h_line, int v_line, HDC hdc) {
	RECT r;
	GetClientRect(WindowFromDC(hdc), &r);
	drawing_context dc{ hdc };
	dc.use_pen(RGB(0, 0, 0));
	dc.line({ 0, v_line }, { r.right, v_line });
	dc.line({ h_line, 0 }, { h_line, r.bottom});
}

void draw_status_text(const trasim::simulator *sim, HDC hdc) {
	RECT client;
	GetClientRect(WindowFromDC(hdc), &client);
	RECT r;
	r.bottom = client.bottom - 20;
	r.left = 15;
	r.right = 350;
	r.top = client.bottom - 40;

	std::wostringstream oss{};
	oss << "West: " << sim->west_spawn_rate() << "%   North: " << sim->north_spawn_rate() << "%";

	DrawText(hdc, oss.str().c_str(), -1, &r, 0);
}
