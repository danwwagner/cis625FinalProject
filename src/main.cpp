#include <iostream>
#include <vector>
#include <omp.h>
#include "Individual.h"
#include "ParameterValues.h"
int main (int argc, char** argv)
{
	if (argc != 4)
	{
		std::cout << "usage: " << argv[0] << " <total cores> <number of lammps cores> <number of individuals>" << std::endl;
		return 1;
	}
	NUM_LAMMPS_THREADS = std::atoi(argv[2]);
	int num_individuals = std::atoi(argv[3]);
	int omp_threads = std::atoi(argv[1]) / NUM_LAMMPS_THREADS;

	omp_set_num_threads(omp_threads);

	std::vector<Individual> individuals;

	int i;
#pragma omp parallel for private(i)
	for (i = 0; i < num_individuals; ++i)
	{
		Individual test;
		std::cout << "i: " << i << " Fitness: " << test.fitness << std::endl;
#pragma omp critical
		{
			individuals.push_back(test);
		}
	}
	return 0;
}
