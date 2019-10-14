#pragma once
#include <vector>
#include <string>
using namespace std;

class Matrix
{
public:
	// Use filename to build matrix
	Matrix(string filename);
	// Use vector to build matrix
	Matrix(vector <vector<int>> mVector);
	// Create an empty array with a size
	Matrix(int height, int width);

	~Matrix();

	vector <vector<int>> mVector;

	// Getters
	size_t height();
	size_t width();

	// Functions
	void save(string filename);

private:
	vector<int> split(string str, char delimiter);
};
