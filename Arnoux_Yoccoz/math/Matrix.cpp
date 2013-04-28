//
//  Matrix.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/21/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "Matrix.h"



void balazs::normalize(Matrix<floating_point_type>& matrix){
    floating_point_type maxValue = matrix.maxEntry();
    assert(maxValue > 0);
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            matrix.setEntry(i, j, matrix.getEntry(i, j)/maxValue);
        }
    }
}
