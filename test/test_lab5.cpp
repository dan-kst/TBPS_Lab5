#include <gtest/gtest.h>
#include "../src/GaussSolver.h"
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>

// Використовуємо "Test Fixture", щоб не дублювати дані
class MatrixIOTest : public ::testing::Test {
protected:
	// --- Helper Struct and Data ---
	struct TestSolver {
		const std::string filename;
		const GaussSolver solver;
		
		// Helper constructor for the TestSolver for convinience
		TestSolver(const std::string filename, GaussSolver s) : filename(filename), solver(s){}	
	};

	// Filename templates
	const std::string DEFAULT_MATRIX = "DEFAULT_MATRIX_";
	const std::string WRONG_MATRIX = "WRONG_MATRIX_";
	const std::string SINGULAR_MATRIX = "SINGULAR_MATRIX_";
	const std::string FILE_TYPE = ".txt";

	// Base data
	std::vector<double> m_base;

	// Our test data vectors
	std::vector<TestSolver> default_matrixes;
	std::vector<std::string> wrong_matrixes_files;

	// --- Our New Helper Function ---
	// Inside MatrixIOTest
	void SetSerializedCorruptedMatrixes(
		const GaussSolver& solver,				// The solver with the *actual* data
		const std::pair<std::string, std::string>& filename_temp, 
		std::vector<std::string>& matrixes_files, 
		const std::vector<std::pair<size_t, size_t>>& dims)			// The *fake* rows and cols to write to the header
	{	
		for(const auto& dim : dims){
			std::string filename = filename_temp.first + 
			std::to_string(dim.first) + "x" + 
			std::to_string(dim.second) + filename_temp.second;
			
			
			std::ofstream outFile(filename);
			if (!outFile.is_open()) {
				throw std::invalid_argument("Error: Could not open file for reading: " + filename);
			}

			// 1. Write the FAKE header
			outFile << dim.first << " " << dim.second << "\n";

			// 2. Write the REAL matrix data
			for (size_t i = 0; i < solver.GetRows(); ++i) {		// Loop over *real* rows
				for (size_t j = 0; j < solver.GetCols(); ++j) {	// Loop over *real* cols
					outFile << solver.Get(i, j) << " ";
				}
				outFile << "\n";
			}
			outFile.close();
			
			matrixes_files.push_back(filename);
		}
	}
	
	void SetSerializedMatrixes(
		GaussSolver& solver,
		const std::pair<std::string, std::string>& filename_temp, 
		std::vector<TestSolver>& matrixes, 
		const std::vector<std::pair<size_t, size_t>>& dims)
	{
		for (const auto& dim : dims) {
			// Set filename of the matrix
			std::string filename = filename_temp.first + 
			std::to_string(dim.first) + "x" + 
			std::to_string(dim.second) + filename_temp.second;
			
			// Set matrix
			solver.SetMatrix(m_base, {6, 6}, dim);
			
			// Write matrix to the file
			solver.Serialize_Matrix_TXT(filename);
			
			// Add matrix and its filename to vector
			matrixes.emplace_back(filename, solver);
		}
	}

	// --- Our Cleaner SetUp() ---
	void SetUp() override {
		// 1. Setup the base data
		m_base = {
			11.1, 5.6, 9.0, 11.8, -5.7, 12.0,
			-17.4, -3.6, 7.0, -1.5, 6.7, 9.5,
			-3.4, -3.6, 2.2, 19.2, 13.5, 8.0,
			17.4, -3.6, 7.1, 4.4, -4.1, -14.4,
			12.0, -2.5, 0.9, 1.8, -3.7, -2.6,
			-1.0, 3.0, 0.25, 0.0, 8.7, 7.6
		};

		GaussSolver solver; // Create the solver *once*

		// 2. Generate all test cases using our helper
		SetSerializedMatrixes(solver, {DEFAULT_MATRIX, FILE_TYPE}, default_matrixes, 
		{{2, 3}, {3, 4}, {5, 6}});

		solver.SetMatrix(m_base, {6, 6}, {6, 6});
		SetSerializedCorruptedMatrixes(solver, {WRONG_MATRIX, FILE_TYPE}, wrong_matrixes_files, 
		{{1, 2}, {3, 2}, {10, 10}});
	}
	
	void Output(const GaussSolver& actual, const GaussSolver& expected, size_t count = 0){
		std::cout << "\nTest :" << std::to_string(count) << std::endl;
		std::cout << "\nExpected: " << std::endl;
		expected.PrintMatrix();
		
		std::cout << "\nActual: " << std::endl;
		actual.PrintMatrix();
	}
};


TEST_F(MatrixIOTest, DeserializesDefaultMatrixCorrectly) {
	GaussSolver actual_solver;
	
	for(const auto& expected : default_matrixes){
		actual_solver.Deserialize_Matrix_TXT(expected.filename);
		
		EXPECT_EQ(actual_solver, expected.solver);
	}
}

TEST_F(MatrixIOTest, DeserializesWrongMatrixCorrectly) {
	GaussSolver actual_solver;
	
	for(const auto& expected : wrong_matrixes_files){
		EXPECT_THROW(
			actual_solver.Deserialize_Matrix_TXT(expected),
			std::runtime_error
		);
	}
}


TEST_F(MatrixIOTest, DeserializesNoFileCorrectly) {
	GaussSolver actual_solver;
	
	EXPECT_THROW(
		actual_solver.Deserialize_Matrix_TXT(""),
		std::invalid_argument
	);
}

class GaussSolverTest : public ::testing::Test{};

TEST_F(GaussSolverTest, GaussSolutionDefaultMatrix){
	GaussSolver sol_actual;
	std::vector<double> mat_expected = {1, 2, 5, 3, 4, 11};
	std::vector<double> sol_expected = {1, 2};
	
	sol_actual.SetMatrix(mat_expected, {2, 3}, {2, 3});
	EXPECT_TRUE(sol_actual.SolveGauss(false));
	
	EXPECT_EQ(sol_actual.GetSolution(), sol_expected);
}

TEST_F(GaussSolverTest, GaussSolutionSingularMatrixNoSolutions){
	GaussSolver sol_actual;
	std::vector<double> mat_expected = {0, 2, 5, 0, 4, 11};
	
	sol_actual.SetMatrix(mat_expected, {2, 3}, {2, 3});
	EXPECT_FALSE(sol_actual.SolveGauss(false));
}

TEST_F(GaussSolverTest, GaussSolutionSingularMatrixInfiniteSolutions){
	GaussSolver sol_actual;
	std::vector<double> mat_expected = {1, 2, 5, 0, 0, 0};
	
	sol_actual.SetMatrix(mat_expected, {2, 3}, {2, 3});
	EXPECT_FALSE(sol_actual.SolveGauss(false));
}
