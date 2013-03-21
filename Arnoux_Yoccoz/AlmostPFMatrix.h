//
//  AlmostPFMatrix.h
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/10/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#ifndef __Arnoux_Yoccoz__AlmostPFMatrix__
#define __Arnoux_Yoccoz__AlmostPFMatrix__

#include <iostream>
#include <deque>
#include <vector>
//#include "Eigen/Eigenvalues"
#include "global.h"
#include <limits>

static const floating_point_type ALLOWED_ERROR = 0.0000000000000001;

/*
template <typename Type>
class SquareMatrix;
*/
template <typename Type>
class Vector;


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
    
    template <typename myType>
    friend Vector<myType> operator*(const Matrix<myType>& matrix, const Vector<myType>& vector);
    friend std::ostream& operator<<(std::ostream& Out, const Matrix<Type>& matrix);

protected:
    std::vector<std::vector<Type>> m_entries;

    template <typename myType>
    friend bool isCloseEnough(const Matrix<myType>& matrix1, const Matrix<myType>& matrix2);
};


void normalize(Matrix<floating_point_type>&);



template <typename Type>
class Vector{
public:
    Vector(int size);
    Vector(const Matrix<Type>& matrix);
    
    template <typename OtherType>
    Vector(const Vector<OtherType>& vector) : m_matrix(vector.m_matrix) {}
    
    int size() const {return m_matrix.height(); }
    
    //std::vector<Type> toStandardVector() const { return m_entries; }
    Type getEntry(int i) const { return m_matrix.getEntry(i, 0); }
    void setEntry(int i, const Type& value) { m_matrix.setEntry(i, 0, value); }
    Type maxEntry() const { return m_matrix.maxEntry(); }
    
    template <typename myType>
    friend Vector<myType> operator*(const Matrix<myType>& matrix, const Vector<myType>& vector);
    friend void normalize(Vector<floating_point_type>& vector);
private:
    Matrix<Type> m_matrix;

};

void normalize(Vector<floating_point_type>& vector) { normalize(vector.m_matrix); }






template <typename Type>
class SquareMatrix{
public:
    SquareMatrix(int size) : m_matrix(size, size) {}
    SquareMatrix(const Matrix<Type>& matrix) : m_matrix(assertSquareMatrix(matrix)) {}

    template <typename OtherType>
    SquareMatrix(const SquareMatrix<OtherType>&);
    
    int size() const { return static_cast<int>( m_matrix.heigth()); }
    Type getEntry(int i, int j) const { return m_matrix.getEntry(i, j); }
    void setEntry(int i, int j, const Type& value) { m_matrix.setEntry(i, j, Value); }
    Type maxEntry() const { return m_matrix.maxEntry(); }
    Vector<Type> column(int i) const { return m_matrix.column(i); }
    
    SquareMatrix<Type> transpose() const { return SquareMatrix<Type>(m_matrix.transpose()); }

    template <typename myType>
    friend SquareMatrix<myType> operator*(const SquareMatrix<myType>& m1, const SquareMatrix<myType>& m2);

    template <typename myType>
    friend Vector<myType> operator*(const SquareMatrix<myType>& matrix, const Vector<myType>& vector);
    friend std::ostream& operator<<(std::ostream& Out, const SquareMatrix<Type>& matrix);

protected:
    Matrix<Type> m_matrix;
    
private:
    const Matrix<Type>& assertSquareMatrix(const Matrix<Type>& matrix);
};




class PerronFrobeniusMatrix{
public:
    PerronFrobeniusMatrix(const std::vector<std::vector<long>>& entries);
    PerronFrobeniusMatrix(const Matrix<long>& intSquareMatrix);
    int size() const{ return m_squareMatrix.height(); }
    long getEntry(int i, int j) const { return m_squareMatrix.getEntry(i, j); }
    PerronFrobeniusMatrix transpose() const { return PerronFrobeniusMatrix(m_squareMatrix.transpose()); }
    bool IsPerronFrobenius() const { return m_perronFrobEigenvector.size() != 0; }
    std::vector<floating_point_type> perronFrobEigenvector();
    floating_point_type perronFrobEigenvalue();
    
    
private:
    Matrix<long> m_squareMatrix;
    std::vector<floating_point_type> m_perronFrobEigenvector;
    floating_point_type m_perronFrobEigenvalue;
    
    Matrix<long> powerUp();
    void initEigenData();
    bool isEigenDataDefined() const { return m_perronFrobEigenvalue != 0; }
    bool isReachableFromEveryVertex(int vertex, const Matrix<long>&) const;
    bool isEveryVertexReachableFrom(int vertex, const Matrix<long>&) const;
    
};

floating_point_type arnouxYoccozStretchFactor(int genus);


#endif /* defined(__Arnoux_Yoccoz__AlmostPFMatrix__) */
