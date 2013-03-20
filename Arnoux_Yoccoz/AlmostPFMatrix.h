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

template <typename Type>
class SquareMatrix;



template <typename Type>
class Vector{
public:
    Vector(const std::vector<Type>& vector);
    Vector(int size);
    
    template <typename OtherType>
    Vector(const Vector<OtherType>&);
    
    std::vector<Type> toStandardVector() const { return m_entries; }
    int size() const { return static_cast<int>(m_entries.size()); }
    Type getEntry(int i) const { return m_entries[i]; }
    void setEntry(int i, const Type& value) { m_entries[i] = value; }
    Type maxEntry() const;
    
    template <typename myType>
    friend Vector<myType> operator*(const SquareMatrix<myType>& matrix, const Vector<myType>& vector);
private:
    std::vector<Type> m_entries;
    static const floating_point_type ALLOWED_ERROR;
    
    template <typename myType>
    friend bool isCloseEnough(const Vector<myType>& vector1, const Vector<myType>& vector2);

};


void normalize(Vector<floating_point_type>&);




template <typename Type>
class SquareMatrix{
public:
    SquareMatrix(int size);
    SquareMatrix(const std::vector<std::vector<Type>>& entries);
    
    template <typename OtherType>
    SquareMatrix(const SquareMatrix<OtherType>&);
    
    int size() const { return static_cast<int>(m_entries.size()); }
    Type getEntry(int i, int j) const { return m_entries[i][j]; }
    void setEntry(int i, int j, const Type& value) { m_entries[i][j] = value; }
    Type maxEntry() const;
    Vector<Type> column(int i) const;
    
    SquareMatrix<Type> transpose() const;

    template <typename myType>
    friend SquareMatrix<myType> operator*(const SquareMatrix<myType>& m1, const SquareMatrix<myType>& m2);

    template <typename myType>
    friend Vector<myType> operator*(const SquareMatrix<myType>& matrix, const Vector<myType>& vector);
    friend std::ostream& operator<<(std::ostream& Out, const SquareMatrix<Type>& matrix);

protected:
    std::vector<std::vector<Type>> m_entries;
};




class PerronFrobeniusMatrix{
public:
    PerronFrobeniusMatrix(const std::vector<std::vector<long>>& entries);
    PerronFrobeniusMatrix(const SquareMatrix<long>& intSquareMatrix);
    int size() const{ return m_squareMatrix.size(); }
    long getEntry(int i, int j) const { return m_squareMatrix.getEntry(i, j); }
    PerronFrobeniusMatrix transpose() const { return PerronFrobeniusMatrix(m_squareMatrix.transpose()); }
    bool IsPerronFrobenius() const { return m_perronFrobEigenvector.size() != 0; }
    std::vector<floating_point_type> perronFrobEigenvector();
    floating_point_type perronFrobEigenvalue();
    
    
private:
    SquareMatrix<long> m_squareMatrix;
    std::vector<floating_point_type> m_perronFrobEigenvector;
    floating_point_type m_perronFrobEigenvalue;
    
    SquareMatrix<long> powerUp();
    void initEigenData();
    bool isEigenDataDefined() const { return m_perronFrobEigenvalue != 0; }
    bool isReachableFromEveryVertex(int vertex, const SquareMatrix<long>&) const;
    bool isEveryVertexReachableFrom(int vertex, const SquareMatrix<long>&) const;
};

floating_point_type arnouxYoccozStretchFactor(int genus);


#endif /* defined(__Arnoux_Yoccoz__AlmostPFMatrix__) */
