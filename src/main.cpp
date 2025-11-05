#include "GaussSolver.h"    // Our GaussSolver class
#include <iostream>    // For std::cout
#include <vector>
#include <filesystem>

int main() {
	/*
	const std::string DESER_PATH = "./Lab5/test_matrix.txt";
	const std::string SER_PATH = "./Lab5/generated_matrix.txt";
	const std::string SOL_PATH = "./Lab5/generated_solution.txt";
	std::cout << "--- Lab 5: Gaussian Elimination ---" << std::endl;

	// --- Test 1: Solve a known 2x2 system from a file ---
	std::cout << "\n--- Test 1: Loading from file 'test_matrix.txt' ---" << std::endl;
	GaussSolver testMatrix(false);

	// Try to load the file
	if (!testMatrix.Deserialize_TXT(DESER_PATH)) {
		std::cerr << "Failed to load '"<<  DESER_PATH << "'. Make sure it's in the right directory." << std::endl;
		return 1; // Exit with an error
	}

	// Print the matrix we just loaded
	std::cout << "Loaded GaussSolver:" << std::endl;
	testMatrix.PrintMatrix();

	// Solve the system
	std::vector<double> solution = testMatrix.SolveGauss();

	// Print the solution (we expect x=1, y=2)
	std::cout << "\nSolution for Test 1:" << std::endl;
	PrintSolution(solution);
	std::cout << "(Expected: x[0] = 1, x[1] = 2)" << std::endl;


	// --- Test 2: Generate, solve, and save a random 4x4 system ---
	std::cout << "\n--- Test 2: Generating a random 4x4 system ---" << std::endl;
	GaussSolver randomMatrix;
	randomMatrix.Generate(4, -10.0, 10.0); // 4x4 system, values from -10 to 10

	std::cout << "Generated GaussSolver:" << std::endl;
	randomMatrix.PrintMatrix();

	// Save the generated matrix
	randomMatrix.Serialize_TXT(SER_PATH);
	std::cout << "\n(Saved to '" << SER_PATH << "')" << std::endl;

	// Solve it
	std::vector<double> randomSolution = randomMatrix.SolveGauss();

	// Print the solution
	std::cout << "\nSolution for Test 2:" << std::endl;
	PrintSolution(randomSolution);

	// Save the solution
	SaveSolutionToFile(randomSolution, SOL_PATH);
	std::cout << "(Solution saved to '" << SOL_PATH << "')" << std::endl;

	std::cout << "\n--- Test 3: Timing a single 1000x1000 run ---" << std::endl;

	GaussSolver largeMatrix;
	const size_t large_size = 1000;

	// 1. Generate data (not timed)
	std::cout << "Generating " << large_size << "x" << large_size << " matrix..." << std::endl;
	largeMatrix.Generate(large_size, -10.0, 10.0);
	std::cout << "Generation complete." << std::endl;

	// 2. Start the timer
	auto start_time = std::chrono::high_resolution_clock::now();

	// 3. Run the solver
	std::vector<double> largeSolution = largeMatrix.SolveGauss();

	// 4. Stop the timer
	auto stop_time = std::chrono::high_resolution_clock::now();

	// 5. Calculate the duration
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

	if (!largeSolution.empty()) {
		std::cout << "Solver finished in " << duration.count() << " milliseconds." << std::endl;
	} else {
		std::cout << "Solver finished (matrix was singular) in " << duration.count() << " milliseconds." << std::endl;
	}


	// --- Test 4: Average time over 10 runs ---
	std::cout << "\n--- Test 4: Averaging 10 runs (" << large_size << "x" << large_size << ") ---" << std::endl;

	const int num_runs = 10;
	std::vector<long long> run_times_ms; // To store durations in milliseconds

	for (int i = 0; i < num_runs; ++i) {
		// We re-generate the matrix each time to prevent any caching
		// and ensure a fair test, but we won't time the generation.
		largeMatrix.Generate(large_size, -10.0, 10.0);

		auto start = std::chrono::high_resolution_clock::now();

		largeMatrix.SolveGauss();

		auto stop = std::chrono::high_resolution_clock::now();

		auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
		std::cout << "Run " << (i + 1) << " took " << time_ms << " ms." << std::endl;
		run_times_ms.push_back(time_ms);
	}

	// Calculate the average
	double average_time = std::accumulate(run_times_ms.begin(), run_times_ms.end(), 0.0) / num_runs;

	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Average solver time: " << average_time << " milliseconds." << std::endl;

	std::cout << "\n--- All tests complete. ---" << std::endl;*/
	return 0;
}
