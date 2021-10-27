#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;


//YOUR CODE GOES HERE!
template<typename T>
class Matrix{
 private:
  int numRows;
  int numColumns;
  vector<T> ** rows;
 public:
  Matrix():numRows(0), numColumns(0), rows(NULL){}

  Matrix(int r, int c):numRows(r), numColumns(c){
  	rows = new vector<T>*[numRows];
	for(int i=0; i<numRows; i++){
		rows[i] = new vector<T>(numColumns);
	}
  }

  Matrix(const Matrix<T> & rhs){
  	numRows = rhs.numRows;
  	numColumns = rhs.numColumns;
  	rows = new vector<T>*[numRows];
	for(int i=0; i<numRows; i++){
		rows[i] = new vector<T>(numColumns);
		*rows[i] = rhs[i];
	}
  }

  ~Matrix(){
  	for(int i=0; i<numRows; i++){
		delete rows[i];
	}
	delete[] rows;
  }

  Matrix & operator=(const Matrix<T> & rhs){
  	if(this != &rhs){
		numRows = rhs.getRows();
		numColumns = rhs.getColumns();
		vector<T>** temp = new vector<T>*[numRows];

		for(int i=0; i<numRows; i++){
			temp[i] = new vector<T>(rhs[i]);
		}

		if(rows != NULL){
			for(int i=0; i<numRows; i++){
				delete rows[i];
			}
			delete[] rows;
		}
		
		rows = temp;
	}
	return *this;
  }

  int getRows() const
  {
  	return numRows;
  }

  int getColumns() const{
  	return numColumns;
  }
  const vector<T> & operator[](int index) const
  {
  	assert(index>=0 && index<numRows);
	return *(rows[index]);
  }

  vector<T> & operator[](int index){
  	assert(index>=0 && index<numRows);
	return *(rows[index]);
  }

  bool operator==(const Matrix<T> & rhs) const{
  	if(numRows != rhs.getRows() || numColumns != rhs.getColumns()) return false;
	for(int i=0; i<numRows; i++){
		if (*rows[i] != *rhs.rows[i]) return false;
	}
	return true;
  }

  Matrix operator+(const Matrix<T> & rhs) const{
  	assert(numRows == rhs.getRows() && numColumns == rhs.getColumns());
	Matrix A(numRows, numColumns);
	for(int i=0; i<numRows; i++){
		for(int j=0; j<numColumns; j++){
			A[i][j] = (*rows[i])[j]+rhs[i][j];
		}
	}
	return A;
  }

};

template <typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs)
{
	if(rhs.getRows() == 0 || rhs.getColumns() == 0){
		s<<"[  ]";
		return s;
	}
	s<<"[ ";
	for(int i=0; i<rhs.getRows()-1; i++){
		cout<<"{";
		for(unsigned j=0; j<rhs[i].size()-1; j++){
			cout << rhs[i][j] << ", ";
		}
		cout<<rhs[i][rhs[i].size()-1]<<"},"<<endl;
	}
	cout<<"{";
	for(unsigned j=0; j<rhs[rhs.getRows()-1].size()-1; j++){
		cout << rhs[rhs.getRows()-1][j] << ", ";
	}
	cout<<rhs[rhs.getRows()-1][rhs[rhs.getRows()-1].size()-1]<<"}"<<" ]";
	return s;
}

#endif
