#pragma once
#include "tlight.h"
#include <limits>

#undef max
namespace trasim {
	struct vector2d {
		int x, y;
	};

	inline vector2d operator+(const vector2d &a, const vector2d &b) {
		return{ a.x + b.x, a.y + b.y };
	}

	inline vector2d operator*(const vector2d &a, int f) {
		return{a.x * f, a.y * f};
	}

	inline vector2d operator-(const vector2d &a, const vector2d &b) {
		return{a.x - b.x, a.y - b.y};
	}

	inline vector2d operator+=(const vector2d &a, const vector2d &b) {
		return{ a.x + b.x, a.y + b.y };
	}

	enum class car_direction {
		HORIZONTAL,
		VERTICAL
	};

	class car {
		vector2d _position;
		vector2d _forward_speed;
		car_direction _direction;
	public:
		car(vector2d initial_position, car_direction direction) {
			_position = initial_position;
			switch (direction) {
			case car_direction::HORIZONTAL:
				_forward_speed = vector2d{ 10, 0 };
				break;
			case car_direction::VERTICAL:
				_forward_speed = vector2d{ 0, 10 };
				break;
			}
		}

		vector2d position() const { return _position; }

		void operator()(int time_step, int distance_to_obstruction = std::numeric_limits<int>::max()) {
			if (distance_to_obstruction < 5) {
				vector2d displace = _forward_speed * time_step;
				_position += displace;
			}
		}

		car_direction direction() const {
			return _direction;
		}
	};

	class simulator {
		std::vector<car> _horizontal_cars;
		std::vector<car> _vertical_cars;
		int _horizontal_signal_pos = 200;
		int _vertical_signal_pos = 200;
		light_signal _horizontal_signal;
		light_signal _vertical_signal;
		int _step_count = 0;

		static const int _time_step = 1;
		static const int _signal_change_limit = 10;

	public:
		simulator() {
			_horizontal_signal.set_state(signal_state::GREEN);
			_vertical_signal.set_state(signal_state::RED);
		}

		inline void add_car(car_direction dir, vector2d initial_pos) {
			switch (dir) {
			case car_direction::HORIZONTAL:
				_horizontal_cars.emplace_back(initial_pos, dir);
				break;
			case car_direction::VERTICAL:
				_horizontal_cars.emplace_back(initial_pos, dir);
				break;
			}
		}

		inline const std::vector<car> &horizontal_cars() const { return _horizontal_cars; }
		inline const std::vector<car> &vertical_cars() const { return _vertical_cars;  }
		inline light_signal &horizontal_signal() { return _horizontal_signal; }
		inline light_signal &vertical_signal() { return _vertical_signal; }

		// simulate system
		inline void operator()() {
			_step_count++;
			if (_step_count % _signal_change_limit == 0) {
				_horizontal_signal.tick();
				_vertical_signal.tick();
			}
			for (auto &c : _horizontal_cars) {
				c(_time_step);
			}
			for (auto &c : _vertical_cars) {
				c(_time_step);
			}
		}
	};

	
}
