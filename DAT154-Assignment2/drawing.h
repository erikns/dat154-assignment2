#pragma once

#include "stdafx.h"

namespace trasim {
	class light_signal;
}

struct point {
	int x, y;
};

struct size {
	int width, height;
};

inline point operator+(const point &p1, const point &p2) {
	return{p1.x + p2.x, p1.y + p2.y};
}

void draw_traffic_light(trasim::light_signal *signal, point position, HDC hdc);
