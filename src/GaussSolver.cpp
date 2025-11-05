#include "GaussSolver.h"

#include <filesystem>
#include <sstream>

GaussSolver::GaussSolver(bool enable_OpenMP)
 : m_size(0), OpenMP_Enabled(enable_OpenMP) {}

size_t GaussSolver::GetSize() const { return m_size; }


std::vector<double>& GaussSolver::operator[](size_t index_i) {
	if (index_i >= m_size) {
		throw std::out_of_range("Index is out of the Vector's range.");
	}
	return matrix[index_i];
}


std::vector<double> GaussSolver::operator[](size_t index_i) const {
	if (index_i >= m_size) {
		throw std::out_of_range("Index is out of the Vector's range.");
	}
	return matrix[index_i];
}

/*
double& GaussSolver::operator[](size_t index_i) {
	if (index >= m_size) {
		throw std::out_of_range("Index is out of the Vector's range.");
	}
	return matrix[index_i][index_j];
}

double GaussSolver::operator[](size_t index_i) const {
	if (index >= m_size) {
		throw std::out_of_range("Index is out of the Vector's range.");
	}
	return matrix[index_i][index_j];
}
*/
// --- Core Functionality ---

void GaussSolver::Generate(size_t size, double min_range, double max_range) {
	m_size = size;

	// 1. Setup the random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(min_range, max_range);

	// 2. Resize the matrix (n rows, n+1 columns)
	matrix.clear();
	matrix.resize(m_size);
	for (std::vector<double>& row : matrix) {
		row.resize(m_size + 1);
	}

	// 3. Fill the matrix with random numbers
	for (size_t i = 0; i < m_size; ++i) {
		for (size_t j = 0; j < m_size + 1; ++j) {
			matrix[i][j] = dist(gen);
		}
	}
}

bool GaussSolver::SolveGauss() {
	// Create a working copy of the data so the original matrix isn't destroyed
	std::vector<std::vector<double>> matrixCopy = matrix;

	// ----------------------------------
	// Part 1: Forward Elimination
	// ----------------------------------
	for (size_t k = 0; k < m_size; ++k) {
		// 1. Find pivot row (partial pivoting)
		size_t abs_max_i = k;
		for (size_t i = k + 1; i < m_size; ++i) {
			if (std::abs(matrixCopy[i][k]) > std::abs(matrixCopy[abs_max_i][k])) {
				abs_max_i = i;
			}
		}

		// 2. Check for singular matrixCopy
		// (Use 1e-10 for floating point 'zero' comparison)
		if (std::abs(matrixCopy[abs_max_i][k]) < 1e-10) {
			return false; // Return false (no unique solution)
		}

		// 3. Swap current row (k) with pivot row (abs_max_i)
		if (abs_max_i != k) {
			matrixCopy[k].swap(matrixCopy[abs_max_i]);
		}

		// 4. Eliminate
		#pragma omp parallel for
		for (size_t i = k + 1; i < m_size; ++i) {
			double factor = matrixCopy[i][k] / matrixCopy[k][k];
			for (size_t j = k; j < m_size + 1; ++j) {
				matrixCopy[i][j] = matrixCopy[i][j] - factor * matrixCopy[k][j];
			}
		}
	}

	// ----------------------------------
	// Part 2: Back Substitution
	// ----------------------------------
	solution.reserve(m_size);

	// 1. Solve for the last variable
	solution[m_size - 1] = matrixCopy[m_size - 1][m_size] / matrixCopy[m_size - 1][m_size - 1];

	// 2. Loop backward from second-to-last row
	for (int i = m_size - 2; i >= 0; --i) {
		double sum = 0.0;
		for (size_t j = i + 1; j < m_size; ++j) {
			sum += matrixCopy[i][j] * solution[j];
		}
		solution[i] = (matrixCopy[i][m_size] - sum) / matrixCopy[i][i];
	}

	return true;
}

// --- Input/Output ---

void GaussSolver::SetMatrix(const std::vector<std::vector<double>>& newMatrix, int rows, int cols){
	if(m_size > 0)
		matrix.clear();
	if((size_t)rows >= newMatrix.size() || (size_t)cols >= newMatrix.size())	
		throw std::out_of_range("Cant Set Matrix: Index is out of range.");
	
	m_size = rows >= cols ? rows : cols;
	matrix.reserve(m_size);
	for(size_t i = 0; i < m_size; i++){
		//matrix[i].reserve(m_size + 1);
		//for(size_t j = 0; j < m_size + 1; j++){
			//matrix[i][j] = (i >= (size_t)rows || j >= (size_t)cols) ? 0.0 : newMatrix[i][j];
		//}
	}
}

void GaussSolver::PrintMatrix() const {
	if (m_size == 0) {
		std::cout << "[ GaussSolver is empty ]" << std::endl;
		return;
	}

	std::cout << "Augmented GaussSolver (" << m_size << "x" << m_size + 1 << "):" << std::endl;
	for (size_t i = 0; i < m_size; ++i) {
		std::cout << "[ ";
		for (size_t j = 0; j < m_size; ++j) {
			std::cout << std::setw(8) << std::fixed << std::setprecision(3) << matrix[i][j] << " ";
		}
		std::cout << "| " << std::setw(8) << std::fixed << std::setprecision(3) << matrix[i][m_size] << " ]\n";
	}
	std::cout.copyfmt(std::ios(NULL)); // Reset cout formatting
}

void PrintSolution(const std::vector<double>& solution) {
    if (solution.empty()) {
        std::cout << "No unique solution exists (matrix is singular)." << std::endl;
        return;
    }

    std::cout << "Solution (x):" << std::endl;
    for (size_t i = 0; i < solution.size(); ++i) {
        std::cout << "x[" << i << "] = " << solution[i] << std::endl;
    }
}

void GaussSolver::Serialize_Solution_TXT(const std::string& filename) const{
    std::ofstream outFile(filename);
  	if (!outFile.is_open()) {
		throw std::invalid_argument("Error: Could not open file for reading: " + filename);
	}

    if (solution.empty()) {
        outFile << "0\n";
        outFile << "No unique solution exists (matrix is singular)." << "\n";
    } else {
        //outFile << "Solution (x): ";
        outFile << std::to_string(solution.size());
        for(const double data : solution)
			outFile << data << " ";
	}
}
void GaussSolver::Deserialize_Solution_TXT(const std::string& filename){
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		throw std::invalid_argument("Error: Could not open file for reading: " + filename);
	}

	// 1. Read size
	inFile >> m_size;
	if (inFile.fail()) 
		throw std::runtime_error("Error during reading first line of the file: " + filename); // Check for read error

	// 2. Resize solution size
	solution.clear();
	solution.resize(m_size);

	// 3. Read solution data
	for (size_t i = 0; i < m_size; i++) {
		inFile >> solution[i];
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
	outFile << m_size << "\n";

	// 2. Write matrix data
	for (size_t i = 0; i < m_size; ++i) {
		for (size_t j = 0; j < m_size + 1; ++j) {
			outFile << matrix[i][j] << (j == m_size ? "" : " ");
			}
		outFile << "\n";
	}

	outFile.close();
}

void GaussSolver::Deserialize_Matrix_TXT(const std::string& filename) {
	std::stringstream current_path;
	current_path << std::filesystem::current_path();
	
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		//std::cout << "Current CWD: " << std::filesystem::current_path() << std::endl; //debug
		throw std::invalid_argument("Error: Could not open file for deserializing the matrix: " + filename + "\nin " + current_path.str());
	}

	// 1. Read size
	inFile >> m_size;
	if (inFile.fail()) 
		throw std::runtime_error("Error during reading first line of the file: " + filename); // Check for read error

	// 2. Resize matrix
	matrix.clear();
	matrix.resize(m_size);
	for (std::vector<double>& row : matrix) {
		row.resize(m_size + 1);
	}

	// 3. Read matrix data
	for (size_t i = 0; i < m_size; ++i) {
		for (size_t j = 0; j < m_size + 1; ++j) {
			inFile >> matrix[i][j];
			if (inFile.fail()) {
				throw std::runtime_error("Error during reading the file: " + filename); // Check for read error
			}
		}
	}

	inFile.close();
}
