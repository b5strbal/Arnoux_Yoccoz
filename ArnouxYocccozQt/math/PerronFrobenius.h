//
//  PerronFrobenius.h
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/10/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#ifndef __Arnoux_Yoccoz__PerronFrobenius__
#define __Arnoux_Yoccoz__PerronFrobenius__

#include <iostream>
#include <vector>
//#include "Eigen/Eigenvalues"
#include "../global.h"
#include <limits>
#include "Matrix.h"

namespace balazs{


class PerronFrobeniusMatrix{
public:
    PerronFrobeniusMatrix(const SquareMatrix<long>& intSquareMatrix);
    int size() const{ return m_squareMatrix.size(); }
    long getEntry(int i, int j) const { return m_squareMatrix.getEntry(i, j); }
    PerronFrobeniusMatrix transpose() const { return PerronFrobeniusMatrix(m_squareMatrix.transpose()); }
    std::vector<floating_point_type> perronFrobEigenvector();
    floating_point_type perronFrobEigenvalue();
    
    
private:
    SquareMatrix<long> m_squareMatrix;
    std::vector<floating_point_type> m_perronFrobEigenvector;
    floating_point_type m_perronFrobEigenvalue;
    
    SquareMatrix<long> powerUp();
    void initEigenData();
    bool isEigenDataDefined() const { return m_perronFrobEigenvalue != 0; }

    
};

floating_point_type arnouxYoccozStretchFactor(int genus);

}


#endif /* defined(__Arnoux_Yoccoz__AlmostPFMatrix__) */
