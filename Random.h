//
// Created by ciamek on 14.06.2021.
//

#ifndef PROJEKT2_RANDOM_H
#define PROJEKT2_RANDOM_H

#include <random>

class Random
{
public:
	Random() = delete;
	Random(const Random &other) = delete;
	Random &operator=(const Random &) = delete;
	
	static int get_int()
	{
		return s_RandomDistribution(s_RandomEngine);
	}
	
	static float get_float()
	{
		return (float)s_RandomDistribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
	
	static double get_double()
	{
		return (double)s_RandomDistribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max();
	}
	
	
private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_RandomDistribution;
};


#endif //PROJEKT2_RANDOM_H
