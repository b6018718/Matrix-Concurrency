#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include "Matrix.h"

using namespace std;

int main() {
	puts("Hello world!");
	// Declare Functions
	Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2);

	// Create the two matrix objects
	Matrix* matrix1 = new Matrix("MatrixInput1.txt");
	Matrix* matrix2 = new Matrix("MatrixInput2.txt");
	// Multiple the matrices 
	Matrix* matrix3 = multiplyMatrices(matrix1, matrix2);
	// Write multiplied matrix to a file
	matrix3->save("MatrixOutput.txt");

	system("pause");

	delete matrix1;
	delete matrix2;
	delete matrix3;
	return 0;
}

Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2) {
	// Check to see if they can be multiplied
	if (matrix1->width() != matrix2->height()) {
		return nullptr;
	}

	// Multiply the matrices
	vector <vector<int>> newMatrixArray;
	// Loop through rows of 1
	for (size_t i = 0; i < matrix1->height(); i++){
		// Loop through columns of 2
		vector <int> row;
		for (size_t j = 0; j < matrix2->width(); j++){
			// Loop through rows of 2
			int coordinateTotal = 0;
			for (size_t k = 0; k < matrix2->height(); k++){
				coordinateTotal += matrix1->mVector[i][k] * matrix2->mVector[k][j];
			}
			row.push_back(coordinateTotal);
		}
		newMatrixArray.push_back(row);
	}
	return new Matrix(newMatrixArray);
}