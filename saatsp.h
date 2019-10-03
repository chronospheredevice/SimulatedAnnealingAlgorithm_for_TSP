#pragma once
#ifdef _WINDOWS
	#ifdef DLL_EXPORT 
	#define DLL_API __declspec(dllexport)
	#else
	#define DLL_API __declspec(dllimport)
	#endif
#endif
#ifdef linux
#define DLL_API
#endif

// These options can be changed before complie
#define NUM float // float, double
constexpr size_t default_total_cycle = 100000;

// Do not change anything below this line
#include <vector>

namespace saatsp {
	class Solver {
	public:
		explicit DLL_API Solver(const std::vector<std::vector<NUM>> &data);
		// input data should be square-like ( data.size() = data[0].size() ) 
		// should be traceless (data[i][i] = 0), should be symmetry (data[i][j] = data[j][i])

		DLL_API void set_temperature_limit(NUM T_max, NUM T_min);
		// should satisfy 0 < T_min < T_max

		DLL_API void get_temperature_limit(NUM &T_max, NUM &T_min) const;

		DLL_API void set_path(const std::vector<size_t> &path);
		// path should be exactly a loop including all vertex (include 0 - data.size()-1 exactly once per each)

		DLL_API std::vector<size_t> get_path() const;

		DLL_API NUM get_path_value() const;
		// will return the path's value in this object now

		DLL_API void set_total_iteration_cycle(size_t total_cycle);
		// total_cycle should not be too large, if so, throw "Total cycle is too large"

		DLL_API size_t get_total_iteration_cycle() const;

		DLL_API void set_temperature_now(NUM T);
		// should satisfy T_min <= T <= T_max

		DLL_API NUM get_temperature_now() const;

		DLL_API void cooldown();
		// cooldown until T <= T_min

		DLL_API void cooldown_cycles(size_t cycles);
		// cooldown <cycles> cycles

	private:
		NUM T_max, T_min, T;
		NUM cool_coefficent;
		size_t total_cycle;
		std::vector<std::vector<NUM>> data;
		std::vector<size_t> path;

		bool check_path(const std::vector<size_t> &path) const;
		bool is_traceless(const std::vector<std::vector<NUM>> &data) const;
		bool is_symmetry(const std::vector<std::vector<NUM>> &data) const;
		NUM get_range();
		NUM get_sdv();		
	};
}