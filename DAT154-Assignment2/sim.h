#pragma once
#include "tlight.h"
#include <limits>

#undef max
namespace trasim {
	struct vector2d {
		int x, y;

		inline vector2d &operator+=(const vector2d &rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		inline vector2d &operator+(const vector2d &rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		inline vector2d &operator-(const vector2d &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
	};

	inline vector2d operator*(const vector2d &a, int f) {
		return{a.x * f, a.y * f};
	}

	/*inline vector2d operator+=(const vector2d &a, const vector2d &b) {
		return{ a.x + b.x, a.y + b.y };
	}*/

	enum class car_direction {
		HORIZONTAL,
		VERTICAL
	};

	class car {
		vector2d _position;
		vector2d _forward_speed;
		car_direction _direction;
	public:
		car(vector2d initial_position, car_direction direction, int speed_scale = 1) {
			_position = initial_position;
			_direction = direction;
			switch (direction) {
			case car_direction::HORIZONTAL:
				_forward_speed = vector2d{ 10, 0 } * speed_scale;
				break;
			case car_direction::VERTICAL:
				_forward_speed = vector2d{ 0, 10 } * speed_scale;
				break;
			}
		}

		vector2d position() const { return _position; }

		void operator()(int time_step, int distance_to_obstruction = std::numeric_limits<int>::max()) {
			if (distance_to_obstruction > 15) {
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

		void set_params(int h_signal_pos, int v_signal_pos) {
			_horizontal_signal_pos = h_signal_pos;
			_vertical_signal_pos = v_signal_pos;
		}

		inline void add_car(car_direction dir, vector2d initial_pos, float speed_scale = 1) {
			switch (dir) {
			case car_direction::HORIZONTAL:
				_horizontal_cars.emplace_back(initial_pos, dir, speed_scale);
				break;
			case car_direction::VERTICAL:
				_vertical_cars.emplace_back(initial_pos, dir, speed_scale);
				break;
			}
		}

		inline const std::vector<car> &horizontal_cars() const { return _horizontal_cars; }
		inline const std::vector<car> &vertical_cars() const { return _vertical_cars;  }
		inline light_signal &horizontal_signal() { return _horizontal_signal; }
		inline light_signal &vertical_signal() { return _vertical_signal; }
		inline int vertical_signal_position() const { return _vertical_signal_pos; }
		inline int horizontal_signal_position() const {
			return _horizontal_signal_pos;
		}

		// simulate system
		inline void operator()() {
			_step_count++;
			if (_step_count % _signal_change_limit == 0) {
				_horizontal_signal.tick();
				_vertical_signal.tick();
			}

			vector2d h_last_pos{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
			for (auto &c : _horizontal_cars) {
				auto distance_to_next_car = h_last_pos.x - c.position().x;
				auto distance_to_signal = _horizontal_signal_pos - c.position().x;
				auto effective_distance = distance_to_next_car;
				if (!_horizontal_signal.is_green() && (distance_to_signal < 15) && (distance_to_signal > 0)) {
					effective_distance = distance_to_signal;
				}
				h_last_pos = c.position();
				c(_time_step, effective_distance);
			}

			vector2d v_last_pos{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
			for (auto &c : _vertical_cars) {
				auto distance_to_next_car = v_last_pos.y - c.position().y;
				auto distance_to_signal = _vertical_signal_pos - c.position().y;
				auto effective_distance = distance_to_next_car;
				if (!_vertical_signal.is_green() && (distance_to_signal < 15) && (distance_to_signal > 0)) {
					effective_distance = distance_to_signal;
				}
				v_last_pos = c.position();
				c(_time_step, effective_distance);
			}
		}
	};

	
}
