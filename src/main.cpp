#include <iostream>
#include <vector>
#include "Individual.h"
int main (int argc, char** argv)
{
	std::vector<Individual> individuals;
	for (int i = 0; i < 100; ++i)
	{
		Individual test;
		std::cout << "i: " << i << " Fitness: " << test.fitness << std::endl;
		individuals.push_back(test);
	}
	return 0;
}
