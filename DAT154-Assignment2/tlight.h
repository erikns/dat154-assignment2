#pragma once

#ifndef __H_TLIGHT__
#define __H_TLIGHT__

namespace trasim {
	enum class signal_state { RED, RED_YELLOW, GREEN, YELLOW };

	inline signal_state next_signal_state(signal_state s) {
		switch (s) {
		case signal_state::RED:
			return signal_state::RED_YELLOW;
		case signal_state::RED_YELLOW:
			return signal_state::GREEN;
		case signal_state::GREEN:
			return signal_state::YELLOW;
		case signal_state::YELLOW:
			return signal_state::RED;
		default:
			return signal_state::RED;
		}
	}

	class light_signal {
		signal_state state = signal_state::RED;

	public:
		void tick() { state = next_signal_state(state); }
		bool is_green() const { return state == signal_state::GREEN; }
		signal_state get_state() const { return state; }
		void set_state(signal_state newState) { state = newState; }
	};
}

#endif
