#include "Matrix.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

Matrix::Matrix(string filename)
{
	// Create an array of integers
	ifstream myfile(filename);
	string line;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector<int> matrixLine = split(line, ' ');
			// Set the classes internal matrix vector
			mVector.push_back(matrixLine);
		}
		myfile.close();
	} else {
		puts("File cannot be opened");
	}
}

// This uses a member initialiser list
Matrix::Matrix(vector<vector<int>> mVector) : mVector(mVector)
{
}

Matrix::Matrix(int height, int width)
{
	for (size_t i = 0; i < height; i++){
		vector <int> newVector;
		mVector.push_back(newVector);
		vector <int> newVectorY;
		for (size_t j = 0; j < width; j++){
			newVectorY.push_back(0);
		}
		mVector[i].push_back(newVectorY);
	}
}

Matrix::~Matrix()
{
}

size_t Matrix::height()
{
	return mVector.size();
}

size_t Matrix::width()
{
	return mVector[0].size();
}

void Matrix::save(string filename)
{
	std::ofstream textFile(filename);
	for (size_t i = 0; i < mVector.size(); i++){
		for (size_t j = 0; j < mVector[i].size(); j++){
			textFile << mVector[i][j] << " ";
		}
		textFile << endl;
	}
	textFile.close();
}

vector<int> Matrix::split(string str, char delimiter) {
	vector<int> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		int integer = stoi(tok);
		internal.push_back(integer);
	}

	return internal;
}

