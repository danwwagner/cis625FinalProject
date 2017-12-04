#include "Individual.h"
#include <string>

using std::string;

class Genetics
{
	private:
		int _numTraits, _popSize;
		float _crossProb, _mutProb;
	public:
		Genetics(float, float, int, int);
		~Genetics()

		void Crossover(Individual&, Individual&); // Crossover of two individuals
		void Mutate(Individual&, string); // Mutation of a single individual
		Individual TournamentSelection(Individual*, int); // Tournament selection of individuals
};
