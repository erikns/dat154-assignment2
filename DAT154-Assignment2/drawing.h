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
	explicit solid_brush(COLORREF color) : _brush{CreateSolidBrush(color)} {}

	// Exclusive resource, so disable copying and moving
	solid_brush(const solid_brush&) = delete;
	solid_brush(solid_brush &&) = delete;
	solid_brush &operator=(const solid_brush&) = delete;
	solid_brush &operator=(solid_brush &&) = delete;

	// Use wrapper as a normal HBRUSH
	operator HBRUSH() const {
		return _brush;
	}

	~solid_brush() throw() {
		VERIFY(DeleteObject(_brush));
	}
};

void draw_traffic_light(trasim::light_signal *signal, point position, float scale, HDC hdc);
