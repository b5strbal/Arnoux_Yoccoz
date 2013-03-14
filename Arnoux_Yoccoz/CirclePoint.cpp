/**
 * @file CirclePoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "CirclePoint.h"




bool operator<(const CirclePoint& c1, const CirclePoint& c2){
    if(c1.m_Position < c2.m_Position)
        return true;
    if(c1.m_Position > c2.m_Position)
        return false;
    if((c1.m_Side == LEFT && (c2.m_Side == RIGHT || c2.m_Side == CENTER)) || (c1.m_Side == CENTER && c2.m_Side == RIGHT))
        return true;
    return false;
}



bool IsBetween(const CirclePoint& cLeft, const CirclePoint& cRight, const CirclePoint& cThird)
{
    if (cLeft <= cThird && cThird <= cRight) {
        return true;
    }
    if (cRight < cLeft && (cLeft <= cThird || cThird <= cRight)) {
        return true;
    }
    return false;
}


floating_point_type DistanceBetween(const CirclePoint& c1, const CirclePoint& c2)
{
    return c1.m_Position <= c2.m_Position ? c2.m_Position - c1.m_Position : c2.m_Position - c1.m_Position + 1;
}