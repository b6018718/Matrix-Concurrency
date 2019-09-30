#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>  

using namespace std;

int main() {
	puts("Hello world!");
	vector <vector<int>> createMatrixArray(string filename);
	vector <vector<int>> matrixArrayPointer = createMatrixArray("MatrixInput1.txt");

	system("pause");
	return 0;
}

vector <vector<int>> createMatrixArray(string filename) {
	//Declare function
	vector<int> split(string str, char delimiter);
	// Create an array of integers
	ifstream myfile("MatrixInput1.txt");
	string line;
	vector <vector<int>> matrixArray;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector<int> matrixLine = split(line, ' ');
			matrixArray.insert(matrixArray.end(), matrixLine.begin(), matrixLine.end());
		}
		myfile.close();
	} else {
		puts("File cannot be opened");
	}
	return matrixArray;
}

vector<int> split(string str, char delimiter) {
	vector<int> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		int integer = stoi(tok);
		internal.push_back(integer);
	}

	return internal;
}