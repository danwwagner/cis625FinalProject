#include "Individual.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using std::string;

class Genetics
{
	private:
		int _numTraits, _popSize;
		float _crossProb, _mutProb;
	public:

		// Initialize the genetic algorithm with the probabilities of mutation and crossover.
		Genetics(float cross, float mut, int traits, int population)
	       	{
			_crossProb = cross;
			_mutProb = mut;
			_numTraits = traits;
			_popSize = population;
			srand(time(NULL));  // initialize random seed
		}

		~Genetics()

		// Commence crossover on two given individuals
		void Crossover(Individual& ind_1, Individual& ind_2)
		{
			// Get the crossover threshold
			auto cross_thresh = rand() % _numTraits;
			auto count = 0;

			// Exchange the traits from the threshold beyond.
			// According to Cantu PDF, it's a simple swap.
			for (auto pair<string, double> : ind_1) {
				if (count >= cross_thresh) {
					auto temp = pair;
					ind_1 = ind_2.parameters[pair.first];
					ind_2 = temp;
				}
				count++;
			}
		}

		// Mutates a single trait of the given individual by recalculating it.
		void Mutate(Individual& ind, string trait) {
			ind.recalc(trait);
		}

		// Iterates through the population to find the best parent.
		Individual TournamentSelection(Individual &population, int size) {
			Individual best = NULL;
			double fitness = 0.0;
			for (int i = 0; i < size; i++) {
				auto sel = rand() % _popSize; 
				Individual test = population[sel];

				// The best parent is the individual with the highest fitness.
				if (test.fitness > fitnes) {
					best = test;
				}	
			}
			return best;
		}

};
