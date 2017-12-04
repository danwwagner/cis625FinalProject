#pragma once
#include "Random.h"
#include "ParameterValues.h"
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using std::unordered_map;
using std::string;
using std::stringstream;

class Individual
{
	private:
		// This maps parameter names to parameter values.
		unordered_map<string, double> parameters;	
		Random r;

		double EvaluateFitness()
		{
			// First, we need to write the pair coefficient data to a temporary file
			std::ofstream tmp;
			// TODO: change this when parallelizing.
			tmp.open("/tmp/Pt.tersoff");		
			tmp << lammps_input_line();
			tmp.close();

			// not a great way to do this, but it works.
			// currently requires a temporary file.
			// TODO: When parallelizing, this will NEED to be looked at.
			std::string line;
			auto fd = popen("/usr/bin/lammps < in.pt | awk '/Energy/{getline; print $3}' > out", "w");
			pclose(fd);

			std::ifstream out;
			out.open("out");
			std::getline(out, line);	

			if (line.empty()) return std::nan("invalid individual");
	
			double energy = std::atof(line.c_str());
			std::cout << "Energy: " << energy << std::endl;
			return energy; // TODO: This is not actually the fitness yet.
		}

	public:
		double fitness = 0.0;

		Individual() 
		{
			// Keep generating parameter sets until LAMMPS decides it's valid.
			// This shouldn't happen very often unless we are really unlucky.
			do 
			{
				parameters = {
					{"C", r.real(C_LOWER,C_UPPER)},
					{"D", r.real(D_LOWER,D_UPPER)},
					{"H", r.real(H_LOWER,H_UPPER)},
					{"N", r.real(N_LOWER,N_UPPER)},
					{"BETA", r.real(BETA_LOWER,BETA_UPPER)},
					{"LAMBDA2", r.real(LAMBDA_2_LOWER,LAMBDA_2_UPPER)},
					{"B", r.real(B_LOWER,B_UPPER)},
					{"R", r.real(R_LOWER,R_UPPER)},
					{"S", r.real(S_LOWER,S_UPPER)},
					{"LAMBDA1", r.real(LAMBDA_1_LOWER,LAMBDA_1_UPPER)},
					{"A", r.real(A_LOWER,A_UPPER)}
				};
				fitness = EvaluateFitness();		
			} while (std::isnan(fitness));
		}
		~Individual() {}

		// Allows this individual to be indexed for a given parameter.
		// Example: individual["C"] returns parameter C.
		double operator[](string parameter) { return parameters[parameter]; }

		// Iterating over this class will iterate over parameter map.
		auto begin() { return parameters.begin(); }
		auto end() { return parameters.end(); }

		// Recalculate a parameter value using the current value as a starting point
		// The new value will be in a range of plus/minus 25% of the current value (VARIATION_RANGE = 0.25)
		void recalc(string parameter)
		{
			auto loc = parameters.find(parameter);
			if (loc == parameters.end())
				throw "recalc(): invalid parameter given.";
			
			do 
			{
				auto upper = parameters[parameter]+(parameters[parameter]*VARIATION_RANGE);
				auto lower = parameters[parameter]-(parameters[parameter]*VARIATION_RANGE);
			
				parameters[parameter] = r.real(lower, upper);

				fitness = EvaluateFitness();
			} while (std::isnan(fitness));	
		}

		// Returns a string containing the pair coefficient input data for LAMMPS.
		string lammps_input_line()
		{
			stringstream ss;
			ss << "Pt Pt Pt 3.0 1.0 0.0 ";
			
			ss << parameters["C"] << " ";
			ss << parameters["D"] << " ";
			ss << parameters["H"] << " ";
			ss << parameters["N"] << " ";
			ss << parameters["BETA"] << " ";
			ss << parameters["LAMBDA2"] << " ";
			ss << parameters["B"] << " ";
			ss << parameters["R"] << " ";
			ss << parameters["S"] << " ";
			ss << parameters["LAMBDA1"] << " ";
			ss << parameters["A"] << " ";
		
			return ss.str();	
		}

};
