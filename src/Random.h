#pragma once
#include <random>

class Random
{
	private:
		std::random_device rd;
		std::mt19937 mt;
	public:
		Random() { mt = std::mt19937(rd()); }
		~Random() {}
		double real(double min, double max)
		{
			std::uniform_real_distribution<double> dist(min, max);
			return dist(mt);
		}
		int integer(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(mt);
		}
};
