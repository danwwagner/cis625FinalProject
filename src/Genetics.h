#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <vector>
#include "Individual.h"
#include "Random.h"

using std::string;

class Genetics
{
	private:
		Random r;
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
		}

		~Genetics() {}

		// Commence crossover on two given individuals
		void Crossover(Individual& ind_1, Individual& ind_2)
		{
			// Get the crossover threshold
			auto cross_thresh = r.integer(0, _numTraits);
			auto count = 0;

			// Exchange the traits from the threshold beyond.
			// According to Cantu PDF, it's a simple swap.
			for (auto param: ind_1) {
				if (count < cross_thresh) {
					count++;
					continue;
				}
				auto temp = param;
				ind_1.setParameter(param.first, ind_2[param.first]);
				ind_2.setParameter(temp);
			}
			ind_1.crossoverFitness();
			ind_2.crossoverFitness();
		}

		// Mutates a single trait of the given individual by recalculating it.
		void Mutate(Individual& ind, string trait) {
			ind.recalc(trait);
		}

		// Grabs the best N parents 
		std::vector<Individual> TournamentSelection(std::vector<Individual> &population, int N) {
			std::vector<Individual> result;
			double fitness = 0.0;
			auto len = population.size();
			for (auto i = 0; i < N; i++) {
				result.push_back(population[i]);
			}
			return result;
		}

		// Rank the individuals by their fitness values.
		// Used by std::sort() of the vector in main.
		static bool RankIndividuals(Individual &first, Individual &second) {
			return (first.fitness < second.fitness);
		}

};
