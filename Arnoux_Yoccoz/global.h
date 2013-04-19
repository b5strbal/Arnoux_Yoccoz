/**
 * @file global.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains global variables used in the program.
 */

#ifndef Arnoux_Yoccoz_global_h
#define Arnoux_Yoccoz_global_h

#include <vector>
#include <cassert>
#include <cmath>


//! \brief   The floating point type used throughout the program for the calculations.
typedef long double floating_point_type;

class ExceptionContainingIntervalNotUnique : public std::runtime_error {};

const int CONTAINING_INTERVAL_NOT_UNIQUE = -100;



enum UpDownDirection{
    UPWARDS = 0,
    DOWNWARDS = 1,
    FIRST = UPWARDS,
    LAST = DOWNWARDS
};




int gcd(int a, int b);





/**
 * @brief   Calculates the fractional part of a real number. E.g. FracPart(2.3) = 0.3, FracPart(-1.8) = 0.2.
 */
floating_point_type FracPart(floating_point_type x);


template <typename Type>
int containingInterval(const std::vector<Type>& separatingPoints, Type point){
    return findInterval(separatingPoints, point, -1, static_cast<int>(separatingPoints.size())  - 1);
}



template <typename Type>
int findInterval(const std::vector<Type>& separatingPoints,
                                      const Type& point,
                                      int start,
                                      int end)
{
    if (start == end){
        if (separatingPoints[start] < point)
            return start;
        else
            return CONTAINING_INTERVAL_NOT_UNIQUE;
        
    }
    int mid = (start + end + 1) / 2;
    if (point < separatingPoints[mid]) {
        return findInterval(separatingPoints, point, start, mid - 1);
    } else
        return findInterval(separatingPoints, point, mid, end);
}




template <typename Type>
std::ostream& operator<<(std::ostream& Out, const std::vector<Type>& vec){
    for (unsigned int i = 0; i < vec.size(); i++) {
        Out << vec[i] << " ";
        }
    return Out;
}

        






#endif
