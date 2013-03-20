//
//  AlmostPFMatrix.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/10/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "AlmostPFMatrix.h"





//--------//
// Vector //
//--------//

template <typename Type>
const floating_point_type Vector<Type>::ALLOWED_ERROR = 0.0000000000000001;



template <typename Type>
Vector<Type>::Vector(const std::vector<Type>& vector) :
    m_entries(vector)
{
    if (vector.size() == 0) {
        throw std::runtime_error("The length of a vector (in linear algebra) must be at least 1.");
    }
}



template <typename Type>
Vector<Type>::Vector(int size) :
    Vector<Type>(std::vector<Type>(size))
{
}





template <typename Type>
template <typename OtherType>
Vector<Type>::Vector(const Vector<OtherType>& vector) :
    m_entries(vector.size())
{
    for (int i = 0; i < size(); i++) {
        m_entries[i] = vector.getEntry(i);
    }
}




template <typename Type>
Vector<Type> operator*(const SquareMatrix<Type>& matrix, const Vector<Type>& vector){
    if (matrix.size() != vector.size()) {
        throw std::runtime_error("The size of a square matrix and a vector must be the same in order to multiply them.");
    }
    const int& size = matrix.size();
    Vector<Type> product(size);
    for (int i = 0; i < size; i++) {
        Type sum = 0;
        for (int k = 0; k < size; k++) {
            sum += matrix.getEntry(i, k) * vector.getEntry(k);
        }
        product.setEntry(i, sum);
    }
    return product;
}



template <typename Type>
Type Vector<Type>::maxEntry() const{
    Type maxValue = 0;
    for (int i = 0; i < size(); i++) {
        if (maxValue < m_entries[i]) {
            maxValue = m_entries[i];
        }
    }
    return maxValue;
}




void normalize(Vector<floating_point_type>& vector){
    floating_point_type maxValue = vector.maxEntry();
    for (int i = 0; i < vector.size(); i++) {
        vector.setEntry(i, vector.getEntry(i)/maxValue);
    }
}



template <typename Type>
bool isCloseEnough(const Vector<Type>& vector1, const Vector<Type>& vector2){
    assert(vector1.size() == vector2.size());
 
    for (int i = 0; i < vector1.size(); i++) {
        if (fabs(vector1.getEntry(i) - vector2.getEntry(i)) > Vector<Type>::ALLOWED_ERROR) {
            return false;
        }
    }
    std::cout.precision(20);
    floating_point_type error = fabs(vector1.getEntry(1) - vector2.getEntry(1));
    std::cout << " " << (error == 0 ? -1 : error) << " ";
    return true;
}






//--------------//
// SquareMatrix //
//--------------//



template <typename Type>
SquareMatrix<Type>::SquareMatrix(const std::vector<std::vector<Type>>& entries){
    if (entries.empty()) {
        throw std::runtime_error("The argument for constucting SquareMatrix can't be empty.");
    }
    if (entries.size() != entries[0].size()) {
        throw std::runtime_error("The height must equal the width for a squarematrix.");
    }
    m_entries = entries;
}


template <typename Type>
SquareMatrix<Type>::SquareMatrix(int size) :
    SquareMatrix<Type>(std::vector<std::vector<Type>>(size, std::vector<Type>(size)))
{
}




template <typename Type>
template <typename OtherType>
SquareMatrix<Type>::SquareMatrix(const SquareMatrix<OtherType>& squareMatrix) :
    m_entries(squareMatrix.size(), std::vector<Type>(squareMatrix.size()))
{
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            m_entries[i][j] = squareMatrix.getEntry(i, j);
        }
    }
}






template <typename Type>
SquareMatrix<Type> operator*(const SquareMatrix<Type>& m1, const SquareMatrix<Type>& m2){
    if (m1.size() != m2.size()) {
        throw std::runtime_error("Square matrices of different sizes are not possible to multiply.");
    }
    const int& size = m1.size();
    SquareMatrix<Type> product(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Type sum = 0;
            for (int k = 0; k < size; k++) {
                sum += m1.getEntry(i, k) * m2.getEntry(k, j);
            }
            product.setEntry(i, j, sum);
        }
    }
    return product;
}




template <typename Type>
std::ostream& operator<<(std::ostream& Out, const SquareMatrix<Type>& matrix){
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.Size(); j++) {
            Out << matrix.m_data[i][j] << " ";
        }
        Out << std::endl;
    }
    return Out;
}



template <typename Type>
SquareMatrix<Type> SquareMatrix<Type>::transpose() const{
    std::vector<std::vector<Type>> copy(size(), std::vector<Type>(size()));
    
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            copy[i][j] = m_entries[j][i];
        }
    }
    return SquareMatrix<Type>(copy);
}




template <typename Type>
Type SquareMatrix<Type>::maxEntry() const{
    Type maxValue = 0;
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            if (maxValue < m_entries[i][j]) {
                maxValue = m_entries[i][j];
            }
        }
    }
    return maxValue;
}




template <typename Type>
Vector<Type> SquareMatrix<Type>::column(int columnIndex) const{
    Vector<Type> myVector(size());
    for (int i = 0; i < size(); i++) {
        myVector.setEntry(i, getEntry(i, columnIndex));
    }
    return myVector;
}










//-----------------------//
// PerronFrobeniusMatrix //
//-----------------------//











PerronFrobeniusMatrix::PerronFrobeniusMatrix(const SquareMatrix<long>& intSquareMatrix) :
    m_squareMatrix(intSquareMatrix),
    m_perronFrobEigenvalue(0),  // initializing the Perron-Frobenius eigenvector and eigenvalue by invalid data
    m_perronFrobEigenvector(0)  // since calculation is costly, we only calculate them when the client asks
{
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

