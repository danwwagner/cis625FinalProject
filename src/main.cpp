#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include "Individual.h"
#include "ParameterValues.h"
#include "Genetics.h"
#include "Random.h"
//#include "simplex.h"

double myclock() 
{
	static time_t t_start = 0;  // Save and subtract off each time

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	if( t_start == 0 ) t_start = ts.tv_sec;

	return (double) (ts.tv_sec - t_start) + ts.tv_nsec * 1.0e-9;
}

int main (int argc, char** argv)
{
	double tstart, ttotal;
	if (argc != 4)
	{
		std::cout << "usage: " << argv[0] << " <total cores> <number of lammps cores> <number of individuals>" << std::endl;
		return 1;
	}
	NUM_LAMMPS_THREADS = std::atoi(argv[2]);
	int num_individuals = std::atoi(argv[3]);
	int omp_threads = std::atoi(argv[1]) / NUM_LAMMPS_THREADS;

	omp_set_num_threads(omp_threads);

	tstart = myclock();	
	tstart = myclock();	

	std::vector<Individual> individuals;
	int i, k, m;
#pragma omp parallel for private(i)
	for (i = 0; i < num_individuals; ++i)
	{
		Individual test;
	//	std::cout << "i: " << i << " Fitness: " << test.fitness << std::endl;
#pragma omp critical
		{
			individuals.push_back(test);
		}
	}


	// Genetic Algorithm -- run on the master node.
	std::vector<Individual> best;
	float converge = 0;
	float prevConverge;
	int fraction = 0;

	// Rank the order of the parameter sets (rank individuals by fitness)
	Genetics genetic_algorithm(0.9, 0.1, 11, num_individuals);
	std::sort(individuals.begin(), individuals.end(), genetic_algorithm.RankIndividuals);
	
	do 
	{
		prevConverge = converge;
	
		// Tournament selection of best two parents
		std::vector<Individual>  parents = genetic_algorithm.TournamentSelection(individuals, 2);

		Random r;
		double c_prob = r.real(0.0, 1.0);
	        double m_prob = r.real(0.0, 1.0);
	//	std::cout << "Parent fitnesses: " << parents[0].fitness << ", " << parents[1].fitness << std::endl;

		// Perform crossover of the parents' traits.
		if (c_prob > 0.1) { genetic_algorithm.Crossover(parents[0], parents[1]); }

		// Perform mutation on a fraction of the individuals.
		if (m_prob <= 0.1) {
	//		std::cout << "Mutation." << std::endl;
			fraction = r.integer(0, num_individuals / 3);
			int trait = r.integer(0, 11);
			string param;
			int count = 0;

			// Find the trait to mutate randomly.
			for (auto pair : parents[0]) {
				if (count == trait) {
					param = pair.first;
					break;
				}
				count++;
			}
		//	std::cout << "Mutation of parameter " << param << " with probability "<< m_prob << std::endl;
		//	std::cout << "Mutating " << fraction << " individuals." << std::endl;

			// Mutate that trait across the fraction of individuals.
			#pragma omp parallel for private(k)	
			for (k = 0; k < fraction; k++) {
		//		std::cout << "Mutating individual #" << (i+1) << std::endl;
				genetic_algorithm.Mutate(individuals[k], param);
			}
		} 

	//	std::cout << "Sorting individuals." << std::endl;
		// Both the old and new individuals are ordered and ranked for fitness.
		std::sort(individuals.begin(), individuals.end(), genetic_algorithm.RankIndividuals);
		best.clear();
		// Retain the best 100 individuals for future generations.
		#pragma omp parallel for private(m)
		for (m = 0; m < 100; m++) {
			converge += individuals[m].fitness;
			#pragma omp critical
			{
				best.push_back(individuals[m]);
			}
		}
		individuals = best;
		converge /= -100;
	//	std::cout << "Convergence check: " << converge << std::endl;
		// Iterate until the genetic algorithm does not find any better individuals -- compare to DFTs?

	} while (converge > prevConverge);

	//Pull the top 20 individuals and use Simplex to locally minimize parameters.
	std::vector<Individual> simplexRuns;
	for (auto k = 0; k < 20; k++) simplexRuns.push_back(best[k]);
	
	// For now, just return the best individual in the set.
	Individual answer = simplexRuns[0];
//	std::cout << "Individual found; fitness: " << answer.fitness << std::endl;
	//using BT::Simplex;
	// TODO: The individual with the lowest objective function (highest fitness) is the answer.
	//Individual answer = Simplex(func, simplexRuns);

   	ttotal = myclock() - tstart;
	std::cout << "Cores, LAMMPS Cores, Individuals, Mutaitons: " << argv[1] << "," << argv[2] << "," << argv[3] << "," << fraction << std::endl;
	std::cout << "Time: " << ttotal << " seconds" << std::endl;

	return 0;
}

