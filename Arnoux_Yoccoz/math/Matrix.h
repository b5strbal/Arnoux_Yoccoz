//
//  Matrix.h
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/21/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#ifndef __Arnoux_Yoccoz__Matrix__
#define __Arnoux_Yoccoz__Matrix__

#include <iostream>
#include <vector>
#include "../global.h"
#include <deque>

namespace balazs{


static const floating_point_type ALLOWED_ERROR = 0.0000000000000001;



template <typename Type>
class Vector;

//--------//
// Matrix //
//--------//


template <typename Type>
class Matrix{
public:
    Matrix(int height, int width);
    Matrix(const std::vector<std::vector<Type>>& entries);
    
    template <typename OtherType>
    Matrix(const Matrix<OtherType>&);
    
    int height() const { return static_cast<int>(m_entries.size()); }
    int width() const { return static_cast<int>(m_entries[0].size()); }
    Type getEntry(int i, int j) const { return m_entries[i][j]; }
    void setEntry(int i, int j, const Type& value) { m_entries[i][j] = value; }
    Type maxEntry() const;
    Vector<Type> column(int i) const;
    
    Matrix<Type> transpose() const;
    
    template <typename myType>
    friend Matrix<myType> operator*(const Matrix<myType>& m1, const Matrix<myType>& m2);
    
    friend std::ostream& operator<<(std::ostream& Out, const Matrix<Type>& matrix);
    
protected:
    std::vector<std::vector<Type>> m_entries;
    
    template <typename myType>
    friend bool isCloseEnough(const Matrix<myType>& matrix1, const Matrix<myType>& matrix2);
};


void normalize(Matrix<floating_point_type>& matrix);


//--------//
// Vector //
//--------//



template <typename Type>
class Vector : public Matrix<Type>{
public:
    Vector(int size) : Matrix<Type>(size, 1) {}
    Vector(const Matrix<Type>& matrix) : Matrix<Type>(assertVector(matrix)) {}
    
    std::vector<Type> toStandardVector() const;
    Type getEntry(int i) const { return this->m_entries[i][0]; }
    void setEntry(int i, const Type& value) { Matrix<Type>::setEntry(i, 0, value); }
    
private:
    const Matrix<Type>& assertVector(const Matrix<Type>& matrix);
};






//--------------//
// SquareMatrix //
//--------------//




template <typename Type>
class SquareMatrix : public Matrix<Type>{
public:
    SquareMatrix(int size) : Matrix<Type>(size, size) {}
    SquareMatrix(const Matrix<Type>& matrix) : Matrix<Type>(assertSquareMatrix(matrix)) {}
    
    template <typename OtherType>
    SquareMatrix(const SquareMatrix<OtherType>& squareMatrix) : Matrix<Type>(squareMatrix) {}
    
    int size() const { return this->height(); }
    SquareMatrix<Type> transpose() const { return Matrix<Type>::transpose(); }
    bool isIrreducible() const;
    bool isPrimitive() const;
private:
    const Matrix<Type>& assertSquareMatrix(const Matrix<Type>& matrix);
    bool backwardGraphSearch(int vertex, std::vector<int>& distances) const;
    bool forwardGraphSearch(int vertex, std::vector<int>& distances) const;
};




#include "Matrix.hpp"

}


#endif /* defined(__Arnoux_Yoccoz__Matrix__) */


