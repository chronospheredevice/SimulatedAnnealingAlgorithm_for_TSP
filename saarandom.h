#pragma once
#include <random>
#include "saatsp.h"

namespace saarandom {
	class Random {
	public:
		explicit Random(size_t int_min, size_t int_max);
		~Random();
		size_t get_int();
		std::pair<size_t, size_t> get_int_pair();
		NUM get_real();
	private:
		std::uniform_int_distribution<size_t> * int_ptr;
		std::uniform_real_distribution<NUM> * real_ptr;
		std::default_random_engine * engine_ptr;
	};
}