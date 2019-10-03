#include "stdafx.h"
#include "saarandom.h"

saarandom::Random::Random(size_t int_min, size_t int_max){
	assert(int_min < int_max);
	std::random_device r;
	engine_ptr = new std::default_random_engine(r());
	int_ptr = new std::uniform_int_distribution<size_t>(int_min, int_max);
	real_ptr = new std::uniform_real_distribution<NUM>(0.0, 1.0);
}

saarandom::Random::~Random(){
	delete int_ptr;
	delete real_ptr;
	delete engine_ptr;
}

size_t saarandom::Random::get_int(){
	return (*int_ptr)(*engine_ptr);
}

std::pair<size_t, size_t> saarandom::Random::get_int_pair(){
	size_t val1, val2;
	val1 = get_int();
	val2 = get_int();
	while (val1 == val2)
		val2 = get_int();
	return std::make_pair(val1, val2);
}

NUM saarandom::Random::get_real(){
	return (*real_ptr)(*engine_ptr);
}
