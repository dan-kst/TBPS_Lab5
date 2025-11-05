#include <gtest/gtest.h>
#include "../src/GaussSolver.h"
#include <vector>
#include <string>

// Використовуємо "Test Fixture", щоб не дублювати дані
class MatrixIOTest : public ::testing::Test {
protected:	
	struct TestMatrix{
		const std::string filename;
		GaussSolver matrix;
		
		// Helper constructor for the TestMatrix for convinience
		TestMatrix(const std::string filename, GaussSolver m) : filename(filename), matrix(m){}
	};

	const std::string TEST_MATRIX_PATH = "./test/";	

	const std::string TEST_MATRIX_1_TXT = "m_def_2x2.txt";
	const std::string TEST_MATRIX_2_TXT = "m_def_3x3.txt";
	const std::string TEST_MATRIX_3_TXT = "m_def_5x5.txt";

	const std::string TEST_MATRIX_4_TXT = "m_wrong_1x2.txt";
	const std::string TEST_MATRIX_5_TXT = "m_wrong_3x2.txt";
	const std::string TEST_MATRIX_6_TXT = "m_wrong_3x3.txt";

	const std::string TEST_MATRIX_7_TXT = "m_singular_3x3.txt";
	const std::string TEST_MATRIX_8_TXT = "m_sing_5x5.txt";
	std::vector<std::vector<double>> m_6x6;

	
	std::vector<TestMatrix> default_matrixes;
	std::vector<TestMatrix> wrong_matrixes;
	std::vector<TestMatrix> singular_matrixes;

	
    void SetUp() override {
		GaussSolver solver(false);
		m_6x6.push_back({11.1, 5.6, 9.0, 11.8, -5.7, 12.0});
		m_6x6.push_back({-17.4, -3.6, 7.0, -1.5, 6.7, 9.5});
		m_6x6.push_back({-3.4, -3.6, 2.2, 19.2, 13.5, 8.0});
		m_6x6.push_back({17.4, -3.6, 7.1, 4.4, -4.1, -14.4});
		m_6x6.push_back({-12.0, -2.5, 0.9, 1.8, -3.7, -2.6});
		m_6x6.push_back({-1.0, 3.0, 0.25, 0.0, 8.7, 7.6});
		
		// maxtrix_default_2x2
		solver.SetMatrix(m_6x6, 2, 2);
		//default_matrixes.emplace_back(TEST_MATRIX_1_TXT, solver);
		// maxtrix_default_3x3
		//solver.SetMatrix(m_6x6, 3, 3);
		//default_matrixes.emplace_back(TEST_MATRIX_2_TXT, solver);
		// maxtrix_default_5x5
		//solver.SetMatrix(m_6x6, 5, 5);
		//default_matrixes.emplace_back(TEST_MATRIX_3_TXT, solver);
		
		
		// maxtrix_wrong_1x2
		//solver.SetMatrix(m_6x6, 1, 2);
		//wrong_matrixes.emplace_back(TEST_MATRIX_3_TXT, solver);
		// maxtrix_wrong_3x2
		//solver.SetMatrix(m_6x6, 3, 2);
		//wrong_matrixes.emplace_back(TEST_MATRIX_3_TXT, solver);
		
		// maxtrix_singular_3x3
		//solver.SetMatrix(m_6x6, 5, 5);
		//singular_matrixes.emplace_back(TEST_MATRIX_3_TXT, solver);
		

	}
};

TEST_F(MatrixIOTest, DeserializeDefaultMatrixNoFile) {
	GaussSolver actual(false);
	actual.Deserialize_Matrix_TXT(TEST_MATRIX_PATH + TEST_MATRIX_1_TXT);
	
	int expectSize = 10;
	int actualSize = actual.GetSize();
	
	EXPECT_EQ(actualSize,expectSize);
	//EXPECT_THROW(actual.Deserialize_Matrix_TXT(TEST_MATRIX_PATH + TEST_MATRIX_1_TXT), std::invalid_argument);
}

TEST_F(MatrixIOTest, DeserializeDefaultMatrixCorrectly) {
	//GaussSolver actual(false);
	
	EXPECT_EQ(1, 1);
	/*
	size_t test_matrixes_size = default_matrixes.size();
	size_t actualSize, expectSize;
	std::cerr << "Hello World" << std::endl;
	
	for(size_t i = 0; i < test_matrixes_size; i++){
		expectSize = default_matrixes[i].matrix.GetSize();

		actual.Deserialize_Matrix_TXT(TEST_MATRIX_PATH + TEST_MATRIX_1_TXT);
		actualSize = actual.GetSize();

		EXPECT_EQ(actualSize, expectSize);

		//for(size_t k = 0; k < actualSize; k++)
			//for(size_t j = 0; j < actualSize; j++)
				//EXPECT_EQ(actual[k][j], default_matrixes[i].matrix[i][j]);
	}*/
}

TEST_F(MatrixIOTest, DeserializeMatrixWrong) {

	EXPECT_EQ(1,1);

}
