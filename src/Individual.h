#pragma once
#include "Random.h"
#include "ParameterValues.h"
#include <unordered_map>
#include <string>
#include <sstream>

using std::unordered_map;
using std::string;
using std::stringstream;

class Individual
{
	private:
		// This maps parameter names to parameter values.
		unordered_map<string, double> parameters;	
		Random r;
	public:
		Individual() 
		{
			parameters = {
				{"C", r.real(C_LOWER,C_UPPER)},
				{"D", r.real(D_LOWER,D_UPPER)},
				{"H", r.real(H_LOWER,H_UPPER)},
				{"BETA", r.real(BETA_LOWER,BETA_UPPER)},
				{"N", r.real(N_LOWER,N_UPPER)},
				{"LAMBDA2", r.real(LAMBDA_2_LOWER,LAMBDA_2_UPPER)},
				{"B", r.real(B_LOWER,B_UPPER)},
				{"R", r.real(R_LOWER,R_UPPER)},
				{"S", r.real(S_LOWER,S_UPPER)},
				{"LAMBDA1", r.real(LAMBDA_1_LOWER,LAMBDA_1_UPPER)},
				{"A", r.real(A_LOWER,A_UPPER)}
			};
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
			
			auto upper = parameters[parameter]+(parameters[parameter]*VARIATION_RANGE);
			auto lower = parameters[parameter]-(parameters[parameter]*VARIATION_RANGE);
			
			parameters[parameter] = r.real(lower, upper);
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
