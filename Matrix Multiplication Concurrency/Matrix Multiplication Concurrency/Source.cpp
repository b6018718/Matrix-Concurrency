#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <math.h>
#include <mutex>
#include "Matrix.h"

using namespace std;

// Structs
struct Tile {
	int startx;
	int endx;
	int starty;
	int endy;
};

// Declare Functions
Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2, int maxThreads);
Matrix* multiplyMatricesUsingTiles(Matrix* matrix1, Matrix* matrix2, int tileSize);
void multiplyMatricesWithRange(Matrix* matrix1, Matrix* matrix2, Matrix* matrix3, Tile tileA, Tile tileB);
// Global variable
std::mutex mtx;

int main() {
	//puts("Hello world!");
	int maxThreads(1);
	bool exit(false);
	while (!exit) {
		//cout << "Maximum number of threads? (Exit = -1)";
		cout << "Tile size? (Exit = -1)";
		cin >> maxThreads;
		if (maxThreads == -1) {
			exit = true;
		} else {
			//Start timer
			auto timer1 = std::chrono::high_resolution_clock::now();


			// Create the two matrix objects
			Matrix* matrix1 = new Matrix("MatrixInput1.txt");
			Matrix* matrix2 = new Matrix("MatrixInput2.txt");
			// Multiple the matrices 
			//Matrix* matrix3 = multiplyMatrices(matrix1, matrix2, maxThreads);
			Matrix* matrix3 = multiplyMatricesUsingTiles(matrix1, matrix2, maxThreads);
			// Write multiplied matrix to a file
			matrix3->save("MatrixOutput.txt");

			delete matrix1;
			delete matrix2;
			delete matrix3;

			auto timer2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timer2 - timer1).count();
			cout << "Time: " << duration << "\n";
		}
	}


	//system("pause");

	return 0;
}

void computeRow(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray, int i, int numberPerThread) {
	// Loop through columns of 2
	for (size_t l = 0; l < numberPerThread; l++) {
		if (i + l < matrix1->height()) {
			for (size_t j = 0; j < matrix2->width(); j++) {
				// Loop through rows of 2
				int coordinateTotal = 0;
				for (size_t k = 0; k < matrix2->height(); k++) {
					coordinateTotal += matrix1->mVector[i + l][k] * matrix2->mVector[k][j];
				}
				newMatrixArray[i + l][j] = coordinateTotal;
			}
		}
	}
}

Matrix* multiplyMatricesUsingTiles(Matrix* matrix1, Matrix* matrix2, int tileSize) {
	// Check to seee if they can be multiplied
	if (matrix1->width() != matrix2->height()) {
		return nullptr;
	}

	// Check to see the number of tiles isn't too big to fit inside the matrix
	if (tileSize > matrix1->height() - 1) {
		return multiplyMatrices(matrix1, matrix2, 1);
	}

	// Create empty matrix to fill
	Matrix* matrix3 = new Matrix(matrix1->height(), matrix2->width());

	int numberOfTilesX = matrix1->height() / tileSize;
	int remainderTiles = matrix1->height() % tileSize;

	// Columns
	/*for (size_t i = 0; i < numberOfTilesX; i++){
		// Rows
		for (size_t j = 0; j < numberOfTilesX; j++){
			Tile tileA;
			tileA.yStart = i * numberOfTilesX;
			tileA.yEnd = (i * numberOfTilesX) + tileSize - 1;
			tileA.xStart = i * numberOfTilesX;
			tileA.xEnd = (j * numberOfTilesX) + tileSize - 1;

			Tile tileB;
			tileB.yStart = j * numberOfTilesX;
			tileB.yEnd = (j * numberOfTilesX) + tileSize - 1;
			tileB.xStart = i * numberOfTilesX;
			tileB.xEnd = (i * numberOfTilesX) + tileSize - 1;
			cout << "Bosman";
			//multiplyMatricesWithRange(matrix1, matrix2, matrix3, destinationTile);
		}
	}*/
	// Loop through rows of 1
	vector <thread> threadVector;
	std::mutex lock;
	
	// Matrix 1 Columns
	for (size_t i = 0; i < numberOfTilesX; i++){
		// Matrix 1 Rows
		for (size_t j = 0; j < numberOfTilesX; j++){
			// Matrix 2 Column
			for (size_t k = 0; k < numberOfTilesX; k++) {
				// Matrix 2 Row = Matrix 1 Column
				Tile matrix1Tile;
				matrix1Tile.startx = (i * tileSize);
				matrix1Tile.starty = (j * tileSize);
				matrix1Tile.endx = (i * tileSize) + tileSize - 1;
				matrix1Tile.endy = (j * tileSize) + tileSize - 1;

				Tile matrix2Tile;
				matrix2Tile.startx = (k * tileSize);
				matrix2Tile.starty = (i * tileSize);
				matrix2Tile.endx = (k * tileSize) + tileSize - 1;
				matrix2Tile.endy = (i * tileSize) + tileSize - 1;

				// Start a new thread
				threadVector.push_back(thread(multiplyMatricesWithRange, std::ref(matrix1), std::ref(matrix2), std::ref(matrix3), matrix1Tile, matrix2Tile));

				// Single threaded
				//multiplyMatricesWithRange(matrix1, matrix2, matrix3, matrix1Tile, matrix2Tile);
			}
		}
	}

	for (size_t i = 0; i < threadVector.size(); i++) {
		threadVector[i].join();
	}
	threadVector.clear();

	return matrix3;
}

void multiplyMatricesWithRange(Matrix* matrix1, Matrix* matrix2, Matrix* matrix3, Tile tileA, Tile tileB) {
	for (size_t i = tileA.starty; i <= tileA.endy; i++) {
		for (size_t j = tileB.startx; j <= tileB.endx; j++) {
			// Loop through rows of 2
			for (size_t k = tileB.starty; k <= tileB.endy; k++) {
				// Multiply the vectors
				mtx.lock();
				matrix3->mVector[i][j] += matrix1->mVector[i][k] * matrix2->mVector[k][j];
				mtx.unlock();
			}
		}
	}

}

Matrix* multiplyMatrices(Matrix* matrix1, Matrix* matrix2, int maxThreads) {
	// Function definition
	void computeRow(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray, int i, int numPerThread);
	void intiaiseVector(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray);
	// Check to see if they can be multiplied
	if (matrix1->width() != matrix2->height()) {
		return nullptr;
	}

	// Multiply the matrices
	vector <vector<int>> newMatrixArray;
	intiaiseVector(matrix1, matrix2, newMatrixArray);

	// Work out how many rows per thread
	int rowsPerThread = 1;
	if (maxThreads != 0 && maxThreads < matrix1->height()) {
		rowsPerThread = ceil((float)matrix1->height() / (float)maxThreads);
	}

	// Loop through rows of 1
	vector <thread> threadVector;
	for (int i = 0; i < matrix1->height();) {
		// Start a new thread
		threadVector.push_back(thread(computeRow, std::ref(matrix1), std::ref(matrix2), std::ref(newMatrixArray), i, rowsPerThread));
		i = i + rowsPerThread;
	}

	for (size_t i = 0; i < threadVector.size(); i++) {
		threadVector[i].join();
	}
	threadVector.clear();



	return new Matrix(newMatrixArray);
}

void intiaiseVector(Matrix* matrix1, Matrix* matrix2, vector <vector<int>>& newMatrixArray) {
	for (size_t i = 0; i < matrix1->height(); i++) {
		vector <int> row(matrix2->width(), 0);
		newMatrixArray.push_back(row);
	}
}
