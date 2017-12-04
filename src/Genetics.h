#include "Individual.h"

class Genetics
{
	private:
		int _numTraits, _popSize;
		float _crossProb, _mutProb;
	public:
		Genetics(float, float, int, int);
		~Genetics()

		void Crossover(Individual, Individual); // Crossover of two individuals
		void Mutate(Individual); // Mutation of a single individual
		Individual TournamentSelection(Individual*, int); // Tournament selection of individuals
};
