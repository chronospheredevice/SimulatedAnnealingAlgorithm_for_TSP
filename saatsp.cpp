// saatsp.cpp : DLLmain Domain
//

#include <cmath>
#include "stdafx.h"
#include "saatsp.h"
#include "saarandom.h"

static constexpr size_t T_max_magicnumber = 10;
static constexpr size_t T_min_magicnumber = 1000;

saatsp::Solver::Solver(const std::vector<std::vector<NUM>> &data) {
	assert(data.size() > 1);
	assert(data.size() == data[0].size());
	assert(is_symmetry(data));
	assert(is_traceless(data));
	this->data = data;
	NUM range = get_range();
	if (range == 0.0) { // fully the same?
		set_temperature_limit(10.0, 1.0);
		set_temperature_now(10.0);
		set_total_iteration_cycle(1);
	} else {
		NUM sdv = get_sdv();
		set_temperature_limit(range * T_max_magicnumber, sdv / T_min_magicnumber);
		set_temperature_now(range * T_max_magicnumber);
		set_total_iteration_cycle(default_total_cycle);
	}
	for (size_t i = 0; i < data.size(); ++i)
		path.push_back(i);
}

void saatsp::Solver::set_temperature_limit(NUM T_max, NUM T_min) {
	assert(0.0 < T_min && T_min < T_max);
	this->T_max = T_max;
	this->T_min = T_min;
}

void saatsp::Solver::get_temperature_limit(NUM &T_max, NUM &T_min) const {
	T_max = this->T_max;
	T_min = this->T_min;
}

void saatsp::Solver::set_path(const std::vector<size_t> &path) {
	assert(check_path(path));
	this->path = path;
}

std::vector<size_t> saatsp::Solver::get_path() const {
	return path;
}

void saatsp::Solver::set_total_iteration_cycle(size_t total_cycle) {
	NUM exp = 1 / NUM(total_cycle);
	cool_coefficent = std::pow(T_min / T_max, exp);
	if (cool_coefficent == 1.0)
		throw "Total cycle is too large";
	this->total_cycle = total_cycle;
}

size_t saatsp::Solver::get_total_iteration_cycle() const {
	return total_cycle;
}

void saatsp::Solver::set_temperature_now(NUM T){
	assert(T >= T_min && T <= T_max);
	this->T = T;
}

NUM saatsp::Solver::get_temperature_now() const {
	return T;
}

void saatsp::Solver::cooldown() {
	saarandom::Random rand(0, data.size() - 1);
	NUM old_val = get_path_value();
	while (T > T_min) {
		auto pair = rand.get_int_pair();
		std::swap(path[pair.first], path[pair.second]);
		NUM new_val = get_path_value();
		if (new_val < old_val) {
			old_val = new_val;
			T *= cool_coefficent;
			continue;
		}
		NUM delta = old_val - new_val;
		NUM probablity = std::exp(delta / T);
		if (rand.get_real() < probablity)
			old_val = new_val;
		else
			std::swap(path[pair.first], path[pair.second]);
		T *= cool_coefficent;
	}
}

void saatsp::Solver::cooldown_cycles(size_t cycles) {
	saarandom::Random rand(0, data.size() - 1);
	NUM old_val = get_path_value();
	for (size_t i = 0; i < cycles && T > T_min; ++i) {
		auto pair = rand.get_int_pair();
		std::swap(path[pair.first], path[pair.second]);
		NUM new_val = get_path_value();
		if (new_val < old_val) {
			old_val = new_val;
			T *= cool_coefficent;
			continue;
		}
		NUM delta = old_val - new_val;
		NUM probablity = std::exp(delta / T);
		if (rand.get_real() < probablity)
			old_val = new_val;
		else
			std::swap(path[pair.first], path[pair.second]);
		T *= cool_coefficent;
	}
}

bool saatsp::Solver::check_path(const std::vector<size_t>& path) const {
	if(path.size() != data.size())
		return false;
	std::vector<bool> reg(path.size(), false);
	for (size_t item : path) {
		if (item >= path.size() || reg[item])
			return false;
		else
			reg[item] = true;
	}
	return true;
}

bool saatsp::Solver::is_traceless(const std::vector<std::vector<NUM>>& data) const {
	for (size_t i = 0; i < data.size(); ++i) {
		if (data[i][i] != 0)
			return false;
	}
	return true;
}

bool saatsp::Solver::is_symmetry(const std::vector<std::vector<NUM>>& data) const {
	for (size_t i = 1; i < data.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (data[i][j] != data[j][i])
				return false;
		}
	}
	return true;
}

NUM saatsp::Solver::get_range(){
	NUM min = data[1][0], max = data[1][0];
	for (size_t i = 1; i < data.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (data[i][j] > max)
				max = data[i][j];
			if (data[i][j] < min)
				min = data[i][j];
		}
	}
	return max - min;
}

NUM saatsp::Solver::get_sdv(){
	NUM sum = 0.0, avg, err = 0.0, error;
	size_t counter = 0;
	for (size_t i = 1; i < data.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			sum += data[i][j];
			++counter;
		}
	}
	avg = sum / counter;
	for (size_t i = 1; i < data.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			error = avg - data[i][j];
			err += error * error;
		}
	}
	err /= counter;
	return std::sqrt(err);
}

NUM saatsp::Solver::get_path_value() const {
	NUM sum = 0.0;
	for (size_t i = 1, j = 0; i < path.size(); ++i, ++j)
		sum += data[path[i]][path[j]];
	sum += data[path[path.size() - 1]][0];
	return sum;
}

