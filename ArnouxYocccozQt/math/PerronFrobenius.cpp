//
//  AlmostPFMatrix.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/10/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "PerronFrobenius.h"
#include <cmath>





//-----------------------//
// PerronFrobeniusMatrix //
//-----------------------//











balazs::PerronFrobeniusMatrix::PerronFrobeniusMatrix(const SquareMatrix<long>& intSquareMatrix) :
    m_squareMatrix(intSquareMatrix),
    m_perronFrobEigenvector(0),  // since calculation is costly, we only calculate them when the client asks
    m_perronFrobEigenvalue(0)  // initializing the Perron-Frobenius eigenvector and eigenvalue by invalid data
{
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            if(getEntry(i, j) < 0){
                throw std::runtime_error("The entries of a Perron-Frobenius matrix must be non-negative.");
            }
        }
    }
    if (!m_squareMatrix.isPrimitive()) {
        throw std::runtime_error("The matrix is not Perron-Frobenius.");
    }
}





balazs::SquareMatrix<long> balazs::PerronFrobeniusMatrix::powerUp(){
    SquareMatrix<long> bigMatrix(m_squareMatrix);
    //int count = 0;
    while (bigMatrix.maxEntry()< sqrt(LONG_MAX/size())) {
        bigMatrix = bigMatrix * bigMatrix;
      //  count++;
    }
    // std::cout << " " << count << " ";
    return bigMatrix;
}






std::vector<long double> balazs::PerronFrobeniusMatrix::perronFrobEigenvector()
{
    if (!isEigenDataDefined()) {
        initEigenData();
    }
    return m_perronFrobEigenvector;
}


long double balazs::PerronFrobeniusMatrix::perronFrobEigenvalue()
{
    if (!isEigenDataDefined()) {
        initEigenData();
    }
    return m_perronFrobEigenvalue;
}






void balazs::PerronFrobeniusMatrix::initEigenData(){
    m_perronFrobEigenvector.resize(size());

    SquareMatrix<long double> hugeMatrix = powerUp();
    Vector<long double> convergingVector = hugeMatrix.column(0);
    normalize(convergingVector);
    
    Vector<long double> previousVector(convergingVector.height());
    int count = 0;
    while (!isCloseEnough(previousVector, convergingVector) && count < 5) {
        previousVector = convergingVector;
        convergingVector = hugeMatrix * convergingVector;
        normalize(convergingVector);
        count++;
    }
   // std::cout << count;
    m_perronFrobEigenvector = convergingVector.toStandardVector();
    
    m_perronFrobEigenvalue = (SquareMatrix<long double>(m_squareMatrix) * convergingVector).getEntry(0, 0) / convergingVector.getEntry(0);

}








long double balazs::arnouxYoccozStretchFactor(int genus){
    SquareMatrix<long double> matrix(genus);
    
    matrix.setEntry(0, 0, 1);
    matrix.setEntry(0, genus - 1, 1);

    for (int i = 1; i < genus - 1; i++) {
        matrix.setEntry(i, i - 1, 1);
    }
    for (int i = 0; i < genus - 1; i++) {
        matrix.setEntry(genus - 1, i, 1);
    }
    return PerronFrobeniusMatrix(matrix).perronFrobEigenvalue();
}

