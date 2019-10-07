#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include "Matrix.h"

using namespace std;

int main() {
	//puts("Hello world!");
	//Start timer
	auto timer1 = std::chrono::high_resolution_clock::now();
	// Declare Functions
	Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2);

	// Create the two matrix objects
	Matrix* matrix1 = new Matrix("MatrixInput1.txt");
	Matrix* matrix2 = new Matrix("MatrixInput2.txt");
	// Multiple the matrices 
	Matrix* matrix3 = multiplyMatrices(matrix1, matrix2);
	// Write multiplied matrix to a file
	matrix3->save("MatrixOutput.txt");

	auto timer2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timer2 - timer1).count();
	cout << "Time: " << duration << "\n";
	system("pause");

	delete matrix1;
	delete matrix2;
	delete matrix3;
	return 0;
}

void computeRow(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray, int i) {
	// Loop through columns of 2
	for (size_t j = 0; j < matrix2->width(); j++) {
		// Loop through rows of 2
		int coordinateTotal = 0;
		for (size_t k = 0; k < matrix2->height(); k++) {
			coordinateTotal += matrix1->mVector[i][k] * matrix2->mVector[k][j];
		}
		newMatrixArray[i][j] = coordinateTotal;
	}
}

Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2) {
	// Function definition
	void computeRow(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray, int i);
	void intiaiseVector(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray);
	// Check to see if they can be multiplied
	if (matrix1->width() != matrix2->height()) {
		return nullptr;
	}

	// Multiply the matrices
	vector <vector<int>> newMatrixArray;
	intiaiseVector(matrix1, matrix2, newMatrixArray);
	// Loop through rows of 1
	vector <thread> threadVector;
	for (int i = 0; i < matrix1->height(); i++){
		//threadVector.push_back(new thread("computeRow", matrix1, matrix2, newMatrixArray, i));
		
		threadVector.push_back(thread(computeRow, matrix1, matrix2, newMatrixArray, i));
	}

	for (size_t i = 0; i < threadVector.size(); i++){
		threadVector[i].join();
	}
	//threadVector.clear();
	
	return new Matrix(newMatrixArray);
}

void intiaiseVector(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray) {
	for (size_t i = 0; i < matrix1->height(); i++){
		vector <int> row(matrix2->width(), 0);
		newMatrixArray.push_back(row);
	}
}
