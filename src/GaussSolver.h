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
 * @brief Represents an augmented matrix for a system of linear equations
 * and implements the Gauss elimination method.
 */
class GaussSolver {
private:
    std::vector<std::vector<double>> matrix;	// Augmented matrix (n x n+1)
    std::vector<double> solution;					// founded solution (n)
    size_t m_size;								// Size 'n' of the system
    const bool OpenMP_Enabled;					// type of computing (sequential or parallel)

public:
	// --- Constructor ---
	
	GaussSolver(bool enable_OpenMP);
	// --- Core Functionality ---

	/**
	 * @brief Generates a random n x (n+1) augmented matrix.
	 */
	void Generate(size_t size, double min_range, double max_range);

	/**
	 * @return dimension size of the matrix.
	 */
	size_t GetSize() const;
	
	/**
	 * @brief Getter for matrix's row by index.
	 * @return vector of double
	 */
	std::vector<double>& operator[](size_t index_i);
	
	/**
	 * @brief Getter for matrix's row by index.
	 * @return row's vector of doubles
	 */
	std::vector<double> operator[](size_t index_i) const;
	
	/**
	 * @brief Getter for matrix's column by index.
	 * @return column's double number
	 */
	//double& operator[](size_t index_i);
	/**
	 * @brief Getter for matrix's row by index.
	 * @return vector of double
	 */
	//double operator[](size_t index_i);

	/**
	 * @brief Solves the system using Gaussian elimination with partial pivoting.
	 * @return true if the solution have been founded, or  false
	 * if no unique solution exists.
	 */
	bool SolveGauss();
	
	// --- Output Functionality ---
	/**
	 * @brief Make copy of the given matrix.
	 */
	void SetMatrix(const std::vector<std::vector<double>>& newMatrix, int rows, int cols);
	
	/**
	 * @brief Prints the current state of the augmented matrix to the console.
	 */
	void PrintMatrix() const;
	

	/**
	 * @brief Prints the solution vector to the console.
	 */
	void GetSolution() const;



	// --- Save/Load Functionality ---
	/**
	 * @brief Saves the solution vector to a text file.
	 * @return true on success, false on file error.
	 */
	void Serialize_Solution_TXT(const std::string& filename) const;
	
	/**
	 * @brief Loads the solution vector from a text file.
	 * @return true on success, false on file error.
	 */
	
	void Deserialize_Solution_TXT(const std::string& filename);

	/**
	 * @brief Saves the matrix to a text file.
	 * Format:
	 * n
	 * a11 a12 ... a1n b1
	 * a21 a22 ... a2n b2
	 * ...
	 * @return true on success, false on file error.
	 */
	void Serialize_Matrix_TXT(const std::string& filename) const;

	/**
	 * @brief Loads the matrix from a text file.
	 * @return true on success, false on file error.
	 */
	void Deserialize_Matrix_TXT(const std::string& filename);
};
