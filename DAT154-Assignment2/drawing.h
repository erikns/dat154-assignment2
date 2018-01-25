#pragma once

#include "stdafx.h"

namespace trasim {
	class light_signal;
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

void draw_traffic_light(trasim::light_signal *signal, point position, float scale, HDC hdc);
