#pragma once

#include "stdafx.h"
#include "debug.h"
#include "sim.h"

namespace trasim {
	class light_signal; // forward declaration
}

struct point {
	int x, y;
};

struct size {
	int x, y;
};

inline point operator+(const point &p1, const point &p2) {
	return{p1.x + p2.x, p1.y + p2.y};
}

template <typename T>
inline T operator*(const T &p1, float f) {
	float x = p1.x * f;
	float y = p1.y * f;
	return{ (int)x, (int)y };
}

// RAII-class for managing the lifetime of a brush
class solid_brush {
	HBRUSH _brush;

public:
	solid_brush() : _brush{ nullptr } {}
	explicit solid_brush(COLORREF color) : _brush{ CreateSolidBrush(color) } { }

	// Exclusive resource, so disable copying
	solid_brush(const solid_brush&) = delete;
	solid_brush(solid_brush &&b) : _brush{ b._brush } { b._brush = nullptr; }
	solid_brush &operator=(const solid_brush&) = delete;
	solid_brush &operator=(solid_brush &&b) { _brush = b._brush; b._brush = nullptr; return *this; }

	// Use wrapper as a normal HBRUSH
	operator HBRUSH() const {
		return _brush;
	}

	~solid_brush() throw() {
		if (_brush)
			VERIFY(DeleteObject(_brush));
	}
};

class solid_pen {
	HPEN _pen;

public:
	solid_pen() : _pen{ nullptr } {}
	explicit solid_pen(COLORREF color) : _pen{ CreatePen(PS_SOLID, 1, color) } { }

	// Exclusive resource, so disable copying
	solid_pen(const solid_pen&) = delete;
	solid_pen(solid_pen &&b) : _pen{ b._pen } { b._pen = nullptr; }
	solid_pen &operator=(const solid_pen&) = delete;
	solid_pen &operator=(solid_pen &&b) { _pen = b._pen; b._pen = nullptr; return *this; }

	// Use wrapper as a normal HBRUSH
	operator HPEN() const {
		return _pen;
	}

	~solid_pen() throw() {
		if (_pen)
			VERIFY(DeleteObject(_pen));
	}
};

// RAII class for managing stack of used GDI objects
class drawing_context {
	HDC _context;
	std::vector<HGDIOBJ> _brushstack;
	std::vector<HGDIOBJ> _penstack;
	solid_brush _brush;
	solid_pen _pen;

public:
	explicit drawing_context(HDC hdc) : _context{ hdc } { }

	drawing_context(const drawing_context&) = delete;
	drawing_context(drawing_context &&) = delete;
	drawing_context &operator=(const drawing_context&) = delete;
	drawing_context &operator=(drawing_context &&) = delete;

	void use_brush(COLORREF color) {
		_brush = solid_brush{ color };
		auto old_brush = SelectObject(_context, _brush);
		_brushstack.push_back(old_brush);
	}

	void use_pen(COLORREF color) {
		_pen = solid_pen{ color };
		auto old_pen = SelectObject(_context, _pen);
		_penstack.push_back(old_pen);
	}

	void pop_brush() {
		auto old_brush = _brushstack.back();
		SelectObject(_context, old_brush);
		_brushstack.pop_back();
	}

	void pop_pen() {
		auto old_pen = _penstack.back();
		SelectObject(_context, old_pen);
		_penstack.pop_back();
	}

	void ellipse(point p, size s) {
		Ellipse(_context, p.x, p.y, p.x + s.x, p.y + s.y);
	}

	void line(point from, point to) {
		MoveToEx(_context, from.x, from.y, nullptr);
		LineTo(_context, to.x, to.y);
	}

	~drawing_context() throw() {
		auto bss = _brushstack.size();
		if (bss > 0) {
			for (auto i = 0u; i < bss; ++i) {
				pop_brush();
			}
		}
		auto pss = _penstack.size();
		if (pss > 0) {
			for (auto i = 0u; i < pss; ++i) {
				pop_pen();
			}
		}
	}
};

void draw_traffic_light(const trasim::light_signal *signal, point position, float scale, HDC hdc);
void draw_cars(const std::vector<trasim::car> &cars, HDC hdc);
void draw_signal_lines(int h_line, int v_line, HDC hdc);
void draw_status_text(const trasim::simulator *sim, HDC hdc);
