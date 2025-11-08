#include "GaussSolver.h"

#include <filesystem>
#include <sstream>

GaussSolver::GaussSolver()
 : m_rows(0), m_cols(0){}
 
GaussSolver::GaussSolver(const GaussSolver& other)
 : 	m_rows(other.m_rows), m_cols(other.m_cols), mat_data(other.mat_data) {}

double GaussSolver::Get(size_t i, size_t j) const{
	if(i >= m_rows || j >= m_cols )
		throw std::out_of_range("Cannot get data: indexes are out of range");
	
	return mat_data[i * m_cols + j];
}

void GaussSolver::Set(size_t i, size_t j, double value){
	if(i >= m_rows || j >= m_cols )
		throw std::out_of_range("Cannot set data: indexes are out of range");
	
	mat_data[i * m_cols + j] = value;
}


size_t GaussSolver::GetRows() const { return m_rows; }

size_t GaussSolver::GetCols() const { return m_cols; }


// --- Set/Get ---

void GaussSolver::SetMatrix(
		const std::vector<double>& source_data, 
		const std::pair<size_t, size_t>& source_dims, 
		const std::pair<size_t, size_t>& copy_dims
){
	if(copy_dims.first > source_data.size() || copy_dims.second > source_data.size())	
		throw std::out_of_range("Cant Set Matrix: Size is out of range.");
	
	m_rows = copy_dims.first;
	m_cols = copy_dims.second;
	mat_data.resize(m_rows * m_cols);

	for(size_t i = 0; i < m_rows; i++){
		for(size_t j = 0; j < m_cols; j++){
			mat_data[i * m_cols + j] = source_data[i * source_dims.second + j];
		}
	}
}

const std::vector<double>& GaussSolver::GetMatrix() const { return mat_data; }


const std::vector<double>& GaussSolver::GetSolution() const { return sol_data; }


bool GaussSolver::operator==(const GaussSolver &rhs) const {
	return 	m_rows == rhs.m_rows &&
			m_cols == rhs.m_cols &&
			mat_data == rhs.mat_data;
}


// --- Core Functionality ---

void GaussSolver::Generate(size_t size, double min_range, double max_range) {
	m_rows = size;
	m_cols = m_rows + 1;

	// 1. Setup the random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(min_range, max_range);

	// 2. Resize the mat_data (n rows, n+1 columns)
	mat_data.clear();
	mat_data.resize(m_rows * m_cols);

	// 3. Fill the mat_data with random numbers
	for (size_t i = 0; i < m_rows; ++i) {
		for (size_t j = 0; j < m_cols; ++j) {
			mat_data[i * m_cols + j] = dist(gen);
		}
	}
}

bool GaussSolver::SolveGauss(const bool& isParallel) {
	// Create a working copy of the data so the original mat_data isn't destroyed
	std::vector<double>& matrixCopy = mat_data;
	// ----------------------------------
	// Part 1: Forward Elimination
	// ----------------------------------
	for (size_t k = 0; k < m_rows; ++k) {
		// 1. Find pivot row (partial pivoting)
		size_t abs_max_i = k;
		size_t better_row_diagonal_value = abs_max_i * m_cols + k;
		for (size_t i = k + 1; i < m_rows; ++i) {
			size_t current_row_diagonal_value = i * m_cols + k;
			if (std::abs(matrixCopy[current_row_diagonal_value]) > std::abs(matrixCopy[better_row_diagonal_value])) {
				abs_max_i = i;
			}
		}

		// 2. Check for singular matrix
		// (Use 1e-10 for floating point 'zero' comparison)
		if (std::abs(matrixCopy[better_row_diagonal_value]) < 1e-10) {
			return false; // Return false (no unique solution)
		}
		
		// 3. Swap current row (k) with pivot row (abs_max_i)
		if (abs_max_i != k) {
			for(size_t j = k; j < m_cols; j++){
				size_t current_pivot_row = k * m_cols + j;
				size_t better_pivot_row = abs_max_i * m_cols + j;
				
				double temp_value = matrixCopy[current_pivot_row];
				matrixCopy[current_pivot_row] = matrixCopy[better_pivot_row];
				matrixCopy[better_pivot_row] = temp_value;
			}
		}
		
		#pragma omp parallel for if(isParallel)
		// 4. Eliminate
		for (size_t i = k + 1; i < m_rows; i++) {
			
			double factor = matrixCopy[i * m_cols + k] / matrixCopy[k * m_cols + k];
			for (size_t j = 0; j < m_cols; ++j) {
				matrixCopy[i * m_cols + j] = matrixCopy[i * m_cols + j] - factor * matrixCopy[k * m_cols + j];
			}
		}
	}

	// ----------------------------------
	// Part 2: Back Substitution
	// ----------------------------------
	size_t last_row_i = m_rows - 1;
	size_t b_col_j = m_cols - 1;
	size_t A_col_j = m_rows - 1;

	size_t b_index = last_row_i * m_cols + b_col_j;
	size_t A_index = last_row_i * m_cols + A_col_j;
	
	sol_data.resize(m_rows);

	// 1. Solve for the last variable
	sol_data[last_row_i] = matrixCopy[b_index] / matrixCopy[A_index];

	// 2. Loop backward from second-to-last row
	for (int i = m_rows - 2; i >= 0; i--) {
		double sum = 0.0;
		for (size_t j = i + 1; j < m_cols; j++) {
			sum += matrixCopy[i * m_cols + j] * sol_data[j];
		}
		sol_data[i] = (matrixCopy[i * m_cols + b_col_j] - sum) / matrixCopy[i * m_cols + i];
	}
	
	return true;
}

// --- Output ---

void GaussSolver::PrintMatrix() const {
	if (m_rows == 0 && m_cols == 0) {
		std::cout << "[ GaussSolver is empty ]" << std::endl;
		return;
	}

	std::cout << "Augmented GaussSolver (" << m_rows << "x" << m_cols << "):" << std::endl;
	for (size_t i = 0; i < m_rows; ++i) {
		std::cout << "[ ";
		for (size_t j = 0; j < m_cols; ++j) {
			std::cout << std::setw(8) << std::fixed << std::setprecision(2) << mat_data[i * m_cols + j];
			if(j == m_cols - 2)
				std::cout << "| ";
			else if(j == m_cols - 1)
				std::cout << " ]\n";
			else
				std::cout  << " ";
		}
	}
	std::cout.copyfmt(std::ios(NULL)); // Reset cout formatting
}

void GaussSolver::PrintSolution() const{
    if (sol_data.empty()) {
        std::cout << "No unique sol_data exists (mat_data is singular)." << std::endl;
        return;
    }

    std::cout << "Solution (x):" << std::endl;
    for (size_t i = 0; i < sol_data.size(); ++i) {
        std::cout << "x[" << i << "] = " << sol_data[i] << std::endl;
    }
}



// --- Save/Load ---

void GaussSolver::Serialize_Solution_TXT(const std::string& filename) const{
    std::ofstream outFile(filename);
  	if (!outFile.is_open()) {
		throw std::invalid_argument("Error: Could not open file for reading: " + filename);
	}

    if (sol_data.empty()) {
        outFile << "0\n";
        outFile << "No unique sol_data exists (mat_data is singular)." << "\n";
    } else {
        //outFile << "Solution (x): ";
        outFile << std::to_string(sol_data.size());
        for(const double data : sol_data)
			outFile << data << " ";
	}
}

void GaussSolver::Deserialize_Solution_TXT(const std::string& filename){
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		throw std::invalid_argument("Error: Could not open file for reading: " + filename);
	}

	// 1. Read size
	inFile >> m_rows;
	if (inFile.fail()) 
		throw std::runtime_error("Error during reading first line of the file: " + filename); // Check for read error

	// 2. Resize sol_data size
	sol_data.clear();
	sol_data.resize(m_rows);

	// 3. Read sol_data data
	for (size_t i = 0; i < m_rows; i++) {
		inFile >> sol_data[i];
		if (inFile.fail()) 
			throw std::runtime_error("Error during reading the file: " + filename); // Check for read error
	}

	inFile.close();
}



void GaussSolver::Serialize_Matrix_TXT(const std::string& filename) const {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		throw std::invalid_argument("Error: Could not open file for reading: " + filename);
	}

	// 1. Write size
	outFile << m_rows << " " << m_cols << "\n";

	// 2. Write mat_data data
	for (size_t i = 0; i < m_rows; ++i) {
		for (size_t j = 0; j < m_cols; ++j) {
			outFile << mat_data[i * m_cols + j] << (j == m_rows ? "" : " ");
			}
		outFile << "\n";
	}

	outFile.close();
}

void GaussSolver::Deserialize_Matrix_TXT(const std::string& filename) {

	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		//std::cout << "Current CWD: " << std::filesystem::current_path() << std::endl; //debug
		throw std::invalid_argument("Error: Could not open file for deserializing the mat_data: " + filename);
	}

	// 1. Read size
	inFile >> m_rows >> m_cols;
	
	if (inFile.fail() || m_rows == 0 || m_cols == 0 || m_cols != m_rows + 1)
		throw std::runtime_error("Error during reading first line of the file: " + filename); // Check for read error

	// 2. Resize matrix
	mat_data.resize(m_rows * m_cols);
	
	// 3. Read matrix data
	for (size_t i = 0; i < m_rows; i++) {
		for (size_t j = 0; j < m_cols; j++) {
			double temp_value;
			inFile >> temp_value;
			if (inFile.fail()) {
				throw std::runtime_error("Error during reading the file: " + filename); // Check for read error
			}
			mat_data[i * m_cols + j] = temp_value;
		}
	}
	
	double test_value;
	inFile >> test_value;
	if(!inFile.fail())
		throw std::runtime_error("Error: the file haven't been read entirely");

	inFile.close();
}
