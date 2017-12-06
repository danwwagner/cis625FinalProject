#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include "Individual.h"
#include "ParameterValues.h"
#include "Genetics.h"
#include "Random.h"

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

	// Genetic Algorithm -- run on the master node.

	// Rank the order of the parameter sets (rank individuals by fitness)
	Genetics genetic_algorithm(0.9, 0.1, 11, num_individuals);
	std::sort(individuals.begin(), individuals.end(), genetic_algorithm.RankIndividuals);
	// Tournament selection of best two parents
	std::vector<Individual>  parents = genetic_algorithm.TournamentSelection(individuals, 2);

	Random r;
	double c_prob = r.real(0.0, 1.0);
        double m_prob = r.real(0.0, 1.0);

	// Perform crossover of the parents' traits.
	if (c_prob > 0.1) genetic_algorithm.Crossover(parents[0], parents[1]);

	if (m_prob <= 0.1) {
		// Perform mutation on a fraction of the individuals.
		// TODO: How to select random trait for mutation?
		int fraction = r.integer(0, num_individuals);
		int trait = r.integer(0, 11);
		string param;
		int count = 0;
		for (auto pair : parents[0]) {
			if (count == trait) {
				param = pair.first;
				break;
			}
			count++;
		}
		for (auto i = 0; i < fraction; i++) {
			genetic_algorithm.Mutate(individuals[i], param);
		}
	}
	// TODO: Both the old and new individuals are ordered and ranked for fitness.	
	return 0;
}

