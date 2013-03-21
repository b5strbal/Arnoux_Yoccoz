//
//  AlmostPFMatrix.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/10/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "AlmostPFMatrix.h"



//--------//
// Matrix //
//--------//


template <typename Type>
Matrix<Type>::Matrix(const std::vector<std::vector<Type>>& entries){
    if (entries.empty()) {
        throw std::runtime_error("A matrix must have at least one row.");
    }
    if (entries[0].empty()) {
        throw std::runtime_error("The width of a matrix must be at least 1.");
    }
    for (int i = 1; i < entries.size(); i++) {
        if (entries[1].size() != entries[0].size()) {
            throw std::runtime_error("In a matrix the length of the rows must be equal.");
        }
    }
    m_entries = entries;
}


template <typename Type>
Matrix<Type>::Matrix(int height, int width) :
    Matrix<Type>(std::vector<std::vector<Type>>(height, std::vector<Type>(width)))
{
}




template <typename Type>
template <typename OtherType>
Matrix<Type>::Matrix(const Matrix<OtherType>& matrix) :
    m_entries(matrix.height(), std::vector<Type>(matrix.width()))
{
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            m_entries[i][j] = matrix.getEntry(i, j);
        }
    }
}






template <typename Type>
Matrix<Type> operator*(const Matrix<Type>& m1, const Matrix<Type>& m2){
    if (m1.width() != m2.height()) {
        throw std::runtime_error("Dimensions of matrices don't match for multiplication.");
    }
    Matrix<Type> product(m1.height(), m2.width());
    for (int i = 0; i < m1.height(); i++) {
        for (int j = 0; j < m2.width(); j++) {
            Type sum = 0;
            for (int k = 0; k < m1.width(); k++) {
                sum += m1.getEntry(i, k) * m2.getEntry(k, j);
            }
            product.setEntry(i, j, sum);
        }
    }
    return product;
}




template <typename Type>
std::ostream& operator<<(std::ostream& Out, const Matrix<Type>& matrix){
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            Out << matrix.m_entries[i][j] << " ";
        }
        Out << std::endl;
    }
    return Out;
}



template <typename Type>
Matrix<Type> Matrix<Type>::transpose() const{
    std::vector<std::vector<Type>> copy(height(), std::vector<Type>(width()));
    
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            copy[i][j] = m_entries[j][i];
        }
    }
    return Matrix<Type>(copy);
}




template <typename Type>
Type Matrix<Type>::maxEntry() const{
    Type maxValue = m_entries[0][0];
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            if (maxValue < m_entries[i][j]) {
                maxValue = m_entries[i][j];
            }
        }
    }
    return maxValue;
}




template <typename Type>
Vector<Type> Matrix<Type>::column(int columnIndex) const{
    if (columnIndex < 0 || columnIndex >= width()) {
        throw std::runtime_error("Invalid column index for Matrix object.");
    }
    Vector<Type> myVector(height());
    for (int i = 0; i < height(); i++) {
        myVector.setEntry(i, getEntry(i, columnIndex));
    }
    return myVector;
}




template <typename Type>
bool isCloseEnough(const Matrix<Type>& matrix1, const Matrix<Type>& matrix2){
    assert(matrix1.width() == matrix1.width());
    assert(matrix1.height() == matrix1.height());

    for (int i = 0; i < matrix1.height(); i++) {
        for (int j = 0; j < matrix2.width(); j++) {
            if (fabs(matrix1.getEntry(i, j) - matrix2.getEntry(i, j)) > ALLOWED_ERROR) {
                return false;
            }
        }
    }
    return true;
}



void normalize(Matrix<floating_point_type>& matrix){
    floating_point_type maxValue = matrix.maxEntry();
    assert(maxValue > 0);
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            matrix.setEntry(i, j, matrix.getEntry(i, j)/maxValue);
        }
    }
}





//--------//
// Vector //
//--------//






template <typename Type>
Vector<Type>::Vector(int size) :
    Matrix<Type>(size, 1)
{
}


template <typename Type>
Vector<Type>::Vector(const Matrix<Type>& matrix) :
    m_matrix(matrix)
{
    if (matrix.width() > 0) {
        throw std::runtime_error("Can't make a vector from a matrix with width larger than 1.");
    }
}



template <typename Type>
Vector<Type> operator*(const Matrix<Type>& matrix, const Vector<Type>& vector){
    if (matrix.width() != vector.size()) {
        throw std::runtime_error("The width of the matrix and a vector must be the same in order to multiply them.");
    }
    return matrix * vector.m_matrix;
}

















//--------------//
// SquareMatrix //
//--------------//




template <typename Type>
const Matrix<Type>& SquareMatrix<Type>::assertSquareMatrix(const Matrix<Type>& matrix){
    if (matrix.height() != matrix.width()) {
        throw std::runtime_error("A SquareMatrix object can only be initialized by a square shaped matrix.");
    }
    return matrix;
}






//-----------------------//
// PerronFrobeniusMatrix //
//-----------------------//











PerronFrobeniusMatrix::PerronFrobeniusMatrix(const Matrix<long>& intSquareMatrix) :
    m_squareMatrix(intSquareMatrix),
    m_perronFrobEigenvalue(0),  // initializing the Perron-Frobenius eigenvector and eigenvalue by invalid data
    m_perronFrobEigenvector(0)  // since calculation is costly, we only calculate them when the client asks
{
    if (m_squareMatrix.width() != m_squareMatrix.height()) {
        throw std::runtime_error("A non-square matrix can't be Perron-Frobenius.");
    }
    
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            if(getEntry(i, j) < 0){
                throw std::runtime_error("The entries of a Perron-Frobenius matrix must be non-negative.");
            }
        }
    }
    if (!isEveryVertexReachableFrom(0, m_squareMatrix) || !isReachableFromEveryVertex(0, m_squareMatrix)) {
        throw std::runtime_error("The matrix is not Perron-Frobenius");
    }
}




PerronFrobeniusMatrix::PerronFrobeniusMatrix(const std::vector<std::vector<long>>& entries) :
    PerronFrobeniusMatrix(SquareMatrix<long>(entries))
{
}







bool PerronFrobeniusMatrix::isEveryVertexReachableFrom(int vertex, const SquareMatrix<long>& matrix) const{
    // Doing a breadth-first search from vectex
    
    assert(vertex >= 0 && vertex < matrix.size());
    int CountReachableVertices = 1;
    
    enum vertex_status{
        ALREADY_SEARCHED_NEIGHBORS,
        NEIGHBORS_TO_BE_SEARCHED,
        NOT_YET_REACHED
    };
    
    std::vector<vertex_status> Status(matrix.size(), NOT_YET_REACHED);
    Status[vertex] = NEIGHBORS_TO_BE_SEARCHED;
    std::deque<int> VerticesToSearch(1, vertex);
    
    while (VerticesToSearch.size() > 0) {
        int CurrentVertex = VerticesToSearch.front();
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix.getEntry(CurrentVertex, i) > 0 && Status[i] == NOT_YET_REACHED) {
                Status[i] = NEIGHBORS_TO_BE_SEARCHED;
                CountReachableVertices++;
                VerticesToSearch.push_back(i);
            }
        }
        Status[CurrentVertex] = ALREADY_SEARCHED_NEIGHBORS;
        VerticesToSearch.pop_front();
    }
    
    if (CountReachableVertices == matrix.size()) {
        return true;
    }
    return false;
}


bool PerronFrobeniusMatrix::isReachableFromEveryVertex(int vertex, const SquareMatrix<long>& matrix) const{
    return isEveryVertexReachableFrom(vertex, matrix.transpose());
}




SquareMatrix<long> PerronFrobeniusMatrix::powerUp(){
    SquareMatrix<long> bigMatrix(m_squareMatrix);
    int count = 0;
    while (bigMatrix.maxEntry()< sqrt(LONG_MAX/size())) {
        bigMatrix = bigMatrix * bigMatrix;
        count++;
    }
    std::cout << " " << count << " ";
    return bigMatrix;
}






std::vector<floating_point_type> PerronFrobeniusMatrix::perronFrobEigenvector() 
{
    if (!isEigenDataDefined()) {
        initEigenData();
    }
    return m_perronFrobEigenvector;
}


floating_point_type PerronFrobeniusMatrix::perronFrobEigenvalue()
{
    if (!isEigenDataDefined()) {
        initEigenData();
    }
    return m_perronFrobEigenvalue;
}






void PerronFrobeniusMatrix::initEigenData(){
    m_perronFrobEigenvector.resize(size());

    SquareMatrix<floating_point_type> hugeMatrix = powerUp();
    Vector<floating_point_type> convergingVector = hugeMatrix.column(0);
    normalize(convergingVector);
    
    Vector<floating_point_type> previousVector(convergingVector.size());
    int count = 0;
    while (!isCloseEnough(previousVector, convergingVector) && count < 5) {
        previousVector = convergingVector;
        convergingVector = hugeMatrix * convergingVector;
        normalize(convergingVector);
        count++;
    }
    std::cout << count;
    m_perronFrobEigenvector = convergingVector.toStandardVector();
    
    m_perronFrobEigenvalue = (SquareMatrix<floating_point_type>(m_squareMatrix) * convergingVector).getEntry(0) / convergingVector.getEntry(0);

}








floating_point_type arnouxYoccozStretchFactor(int genus){
    std::vector<std::vector<long>> Matrix(genus, std::vector<long>(genus));
    
    Matrix[0][0] = 1;
    Matrix[0][genus - 1] = 1;
    for (long i = 1; i < genus - 1; i++) {
        Matrix[i][i - 1] = 1;
    }
    for (long i = 0; i < genus - 1; i++) {
        Matrix[genus - 1][i] = 1;
    }
    return PerronFrobeniusMatrix(Matrix).perronFrobEigenvalue();
}

