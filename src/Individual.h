#pragma once
#include "Random.h"
#include "ParameterValues.h"
#include <unordered_map>
#include <string>
#include <cstring>
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
			string id = std::to_string(r.integer(0,1000000));
			tmp.open("/tmp/Pt.tersoff." + id);		
			tmp << lammps_input_line();
			tmp.close();

			// First, we need to run lammps using the Ptbcc data.
			string lammps_cmd = "/usr/bin/mpirun -n "+std::to_string(NUM_LAMMPS_THREADS)+" /usr/bin/lammps";
			string awk_cmd = "awk '/Energy/{getline; print $3}'";
			string inPtbcc = "printf 'dimension 3\n"
							 "boundary p p p\n"
							 "units real\n"
							 "atom_style charge\n"
							 "read_data dataPtbcc.in\n"
							 "pair_style tersoff\n"
							 "pair_coeff * * /tmp/Pt.tersoff."+id+" Pt\n"
							 "neighbor 2 bin\n"
							 "neigh_modify every 10 delay 0 check no\n"
							 "minimize 1.0e-4 1.0e-6 100 1000\n"
							 "timestep 0.25\n"
							 "run 100\n'";
			stringstream ss;
			ss << inPtbcc << " | ";
			ss << lammps_cmd << " | ";
			ss << awk_cmd;

			auto fd = popen(ss.str().c_str(), "r");
			char buffer[BUFSIZ];
			std::fgets(buffer, sizeof(buffer), fd);			
			pclose(fd);

			if (buffer[0] == '\0') return std::nan("invalid individual");	

			double ptbcc = std::atof(buffer);

			// Next, do the same thing but with the 0.97Ptbcc data.
			std::memset(buffer, 0, sizeof(buffer));
			ss.str("");

			string inx97Ptbcc = "printf 'dimension 3\n"
							  "boundary p p p\n"
							  "units real\n"
							  "atom_style charge\n"
							  "read_data data97xPtbcc.in\n"
							  "pair_style tersoff\n"
							  "pair_coeff * * /tmp/Pt.tersoff."+id+" Pt\n"
							  "neighbor 2 bin\n"
							  "neigh_modify every 10 delay 0 check no\n"
							  "minimize 1.0e-4 1.0e-6 100 1000\n"
							  "timestep 0.25\n"
							  "run 100\n'";
			ss << inx97Ptbcc << " | ";
			ss << lammps_cmd << " | ";
			ss << awk_cmd;

			fd = popen(ss.str().c_str(), "r");
			std::fgets(buffer, sizeof(buffer), fd);
			pclose(fd);

			// std::cout << "Buffer2: " << buffer << std::endl;
			
			// Remove temp file.
			std::remove(("/tmp/Pt.tersoff."+id).c_str());

			if (buffer[0] == '\0') return std::nan("invalid individual");
			double x97Ptbcc = std::atof(buffer);	

			// Finally, perform the fitness calculations.
			double diff = ptbcc - x97Ptbcc;
			

			return (-1)*(DFT_VALUE-diff)*(DFT_VALUE-diff);
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
		Individual (const Individual& other)
		{
			fitness = other.fitness;	
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
