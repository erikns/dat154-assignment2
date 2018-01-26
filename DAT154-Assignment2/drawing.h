#pragma once

#include "stdafx.h"
#include "debug.h"

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
	explicit solid_brush(COLORREF color) : _brush{ CreateSolidBrush(color) } { TRACE(L"Create brush\n"); }

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
		TRACE(L"Destroy brush\n");
		if (_brush)
			VERIFY(DeleteObject(_brush));
	}
};

// RAII class for managing stack of used GDI objects
class drawing_context {
	HDC _context;
	std::vector<HGDIOBJ> _brushstack;
	solid_brush _brush;

public:
	explicit drawing_context(HDC hdc) : _context{ hdc } { TRACE(L"Create context\n"); }

	drawing_context(const drawing_context&) = delete;
	drawing_context(drawing_context &&) = delete;
	drawing_context &operator=(const drawing_context&) = delete;
	drawing_context &operator=(drawing_context &&) = delete;

	void use_brush(COLORREF color) {
		_brush = solid_brush{ color };
		auto old_brush = SelectObject(_context, _brush);
		_brushstack.push_back(old_brush);
	}

	void pop_brush() {
		auto old_brush = _brushstack.back();
		SelectObject(_context, old_brush);
		_brushstack.pop_back();
	}

	void ellipse(point p, size s) {
		Ellipse(_context, p.x, p.y, p.x + s.x, p.y + s.y);
	}

	~drawing_context() throw() {
		TRACE(L"Destroy context\n");
		auto sz = _brushstack.size();
		if (sz > 0) {
			for (auto i = 0u; i < sz; ++i) {
				pop_brush();
			}
		}
	}
};

void draw_traffic_light(trasim::light_signal *signal, point position, float scale, HDC hdc);
