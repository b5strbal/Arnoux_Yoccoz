/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "UnitIntervalPoint.h"
 

const floating_point_type UnitIntervalPoint::PRECISION = 0.00000000001L;



UnitIntervalPoint operator+(const UnitIntervalPoint& p1, floating_point_type distance){
    return UnitIntervalPoint(FracPart(p1.m_position + distance));
}


UnitIntervalPoint operator-(const UnitIntervalPoint& p1, floating_point_type distance){
    return p1 + (-distance);
}




bool operator<(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2){
    if(p1.m_positionPlusPrecision < p2.m_position)
        return true;
    return false;
}



floating_point_type distanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
{
    return p1.m_position <= p2.m_position ? p2.m_position - p1.m_position : p2.m_position - p1.m_position + 1;
}



/*

bool IsBetween(const UnitIntervalPoint& cLeft, const UnitIntervalPoint& cRight, const UnitIntervalPoint& cThird)
{
    if (cLeft <= cThird && cThird <= cRight) {
        return true;
    }
    if (cRight < cLeft && (cLeft <= cThird || cThird <= cRight)) {
        return true;
    }
    return false;
}
*/