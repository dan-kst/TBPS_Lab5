#pragma once

#include <vector>
#include <string>
#include <random>    // For std::mt19937, std::random_device, etc.
#include <fstream>   // For std::ofstream, std::ifstream
#include <iostream>  // For std::cout, std::cerr
#include <cmath>     // For std::abs
#include <iomanip>   // For std::setw (in PrintMatrix)
#include <omp.h>
/**
 * @brief Represents an augmented mat_data for a system of linear equations
 * and implements the Gauss elimination method.
 */
class GaussSolver {
private:
    std::vector<double> mat_data;	// Augmented mat_data (n x n+1)
    std::vector<double> sol_data;	// founded sol_data (n)
    size_t m_rows;					// Size 'n' of the system
    size_t m_cols;					// Size 'n' of the system

public:
	// --- Constructor ---
	// We'll need a constructor to set dimensions
    GaussSolver(int rows, int cols);
	GaussSolver();
	GaussSolver(const GaussSolver &other);


    // --- Getters/ Setters ---
    
    double Get(size_t i, size_t j) const;
    void Set(size_t i, size_t j, double value);

	size_t GetRows() const;
	size_t GetCols() const;
	
	const std::vector<double>& GetMatrix() const;
	/**
	 * @brief Make copy of the given matrix.
	 */
	void SetMatrix(const std::vector<double>& newMatrix, size_t rows, size_t cols);
	
	/**
	 * @brief Prints the solution vector to the console.
	 */
	void GetSolution() const;
	
	bool operator==(const GaussSolver &rhs) const;
	
	// --- Core Functionality ---
	/**
	 * @brief Generates a random n x (n+1) augmented mat_data.
	 */
	void Generate(size_t size, double min_range, double max_range);

	/**
	 * @return dimension size of the mat_data.
	 */

	/**
	 * @brief Solves the system using Gaussian elimination with partial pivoting.
	 * @return true if the solution have been founded, or  false
	 * if no unique solution exists.
	 */
	bool SolveGauss(const bool& isParallel);
	
	// --- Output Functionality ---
	
	/**
	 * @brief Prints the current state of the augmented matrix to the console.
	 */
	void PrintMatrix() const;
	/**
	 * @brief Prints the calculated.
	 */
	void PrintSolution() const;
	

	// --- Save/Load Functionality ---
	/**
	 * @brief Saves the solution vector to a text file.
	 */
	void Serialize_Solution_TXT(const std::string& filename) const;
	
	/**
	 * @brief Loads the solution vector from a text file.
	 */
	
	void Deserialize_Solution_TXT(const std::string& filename);

	/**
	 * @brief Saves the matrix to a text file.
	 * Format:
	 * rows cols
	 * a11 a12 ... a1n b1
	 * a21 a22 ... a2n b2
	 * ...
	 */
	void Serialize_Matrix_TXT(const std::string& filename) const;

	/**
	 * @brief Loads the matrix from a text file.
	 */
	void Deserialize_Matrix_TXT(const std::string& filename);
};
