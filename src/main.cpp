#include "GaussSolver.h"    // Our GaussSolver class
#include <iostream>    // For std::cout
#include <vector>
#include <chrono>

int main() {
	GaussSolver solver;
	std::vector<long long> timer_records;
	const int test_counts = 10;
	timer_records.resize(test_counts*2);
	
	std::cout << "----- STARTING THE TEST -----\n";
	
	std::cout << "----- SMALL TEST CALCULATION -----\n";
	for(int i = 2; i < test_counts - 2; i++){
		std::string matrix_filename = 
				"result_matrix_"
				+ std::to_string(i) + "x" 
				+ std::to_string(i + 1);
		
		std::string solution_filename = 
				"result_solution_"
				+ std::to_string(i) + "x" 
				+ std::to_string(i + 1);
		
		solver.Generate((size_t)i, -50.0, 50.0);
		solver.SolveGauss(false);
		solver.Serialize_Matrix_TXT(matrix_filename);
		std::cout << matrix_filename << " was saved successfully!" << std::endl;
		solver.Serialize_Solution_TXT(solution_filename);
		std::cout << solution_filename << " was saved successfully!" << std::endl << std::endl;
	}
	
	std::cout << "\n\n\n----- BIG TEST CALCULATION -----\n";
	std::cout << "\n\n----- SEQUANTIAL CALCULATION -----\n\n";
	solver.Generate(1000, -100, 100);
	// ----- SEQUANTUAL CALCULATIONS -----
	for(int i = 0; i < test_counts; i++){
		// 1. Get the time *before* the work
		auto start_time = std::chrono::high_resolution_clock::now();

		// 2. Do the work
		solver.SolveGauss(false);

		// 3. Get the time *after* the work
		auto end_time = std::chrono::high_resolution_clock::now();

		// 4. Calculate the duration
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

		// Add the time in microseconds
		timer_records[i] = duration.count();
		std::cout << "Test " + std::to_string(i + 1) + " finished in " 
			+ std::to_string(timer_records[i]) + "ms.\n";
	}
	
	
	std::cout << "\n\n----- PARALLEL(OpenMP) CALCULATION -----\n\n";
	// ----- PARALLEL CALCULATIONS -----
	for(int i = 0; i < test_counts; i++){
		// 1. Get the time *before* the work
		auto start_time = std::chrono::high_resolution_clock::now();

		// 2. Do the work
		solver.SolveGauss(true);

		// 3. Get the time *after* the work
		auto end_time = std::chrono::high_resolution_clock::now();

		// 4. Calculate the duration
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

		// Add the time in microseconds
		timer_records[i + test_counts] = duration.count();
		std::cout << "Test " + std::to_string(i + 1) + " finished in "
			+ std::to_string(timer_records[i + test_counts]) + "ms.\n";
	}
	
	long double seq_aver_time = 0.0;
	long double paral_aver_time = 0.0;
	
	short leftw = 10;
	short rightw = 10;
	short centerw = 5;
	short prec = 2;
	
	std::cout << "\nFinal Results:\n";
	std::cout << std::setw(leftw) << "SEQUANTUAL";
	std::cout << std::setw(centerw) << "|";
	std::cout << std::setw(rightw) << "PARALLEL";
	std::cout << std::endl;
	
	for(size_t i = 0; i < test_counts; i++){
		seq_aver_time += (long double)timer_records[i];
		paral_aver_time += (long double)timer_records[i + test_counts];
		std::cout << std::setw(leftw) << (std::to_string(timer_records[i]) + "ms");
		std::cout << std::setw(centerw) << "|";
		std::cout << std::setw(rightw) << (std::to_string(timer_records[i + test_counts]) + "ms");
		std::cout << std::endl;
	}
	seq_aver_time /= (long double)test_counts;
	paral_aver_time /= (long double)test_counts;
	
	std::cout << "\nAverage:\n";
	std::cout << std::setw(leftw - prec) << std::fixed << std::setprecision(prec) << seq_aver_time << "ms";
	std::cout << std::setw(centerw) << "|";
	std::cout << std::setw(rightw - prec) << std::fixed << std::setprecision(prec) << paral_aver_time << "ms";
	std::cout << std::endl;
	return 0;
}
