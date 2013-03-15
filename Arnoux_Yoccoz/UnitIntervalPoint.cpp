/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "UnitIntervalPoint.h"



UnitIntervalPoint operator+(const UnitIntervalPoint& p1, floating_point_type distance){
    return UnitIntervalPoint(UnitIntervalPoint::FracPart(p1.m_position + distance));
}


UnitIntervalPoint operator-(const UnitIntervalPoint& p1, floating_point_type distance){
    return p1 + (-distance);
}




bool operator<(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2){
    if(p1.m_position < p2.m_position)
        return true;
    if(p1.m_position > p2.m_position)
        return false;
    if(p1.m_epsilon < p2.m_epsilon)
        return true;
    return false;
}



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


floating_point_type DistanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
{
    return p1.m_position <= p2.m_position ? p2.m_position - p1.m_position : p2.m_position - p1.m_position + 1;
}