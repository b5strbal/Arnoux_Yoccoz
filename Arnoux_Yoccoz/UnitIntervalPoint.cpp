/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "UnitIntervalPoint.h"
 

const floating_point_type UnitIntervalPoint::PRECISION = 0.00000000001L;



UnitIntervalPoint operator+(const UnitIntervalPoint& p1, floating_point_type distance){
    return UnitIntervalPoint(FracPart(p1.m_position + distance), p1.m_epsilon);
}


UnitIntervalPoint operator-(const UnitIntervalPoint& p1, floating_point_type distance){
    return p1 + (-distance);
}




bool operator<(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2){
    if(p1.m_positionPlusPrecision < p2.m_position || (p1.m_position == p2.m_position && p1.m_epsilon < p2.m_epsilon))
        return true;
    return false;
}



floating_point_type distanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
{
    return p1.m_position <= p2.m_position ? p2.m_position - p1.m_position : p2.m_position - p1.m_position + 1;
}


std::string UnitIntervalPoint::print() const {
    std::ostringstream s;
    s << m_position;
    if(m_epsilon > 0)
        s << "(+" << m_epsilon << "eps)";
    else if(m_epsilon < 0)
        s << "(" << m_epsilon << "eps)";
    return s.str();
}

std::ostream& operator<<(std::ostream& Out, const UnitIntervalPoint& p){
    Out << p.print();
    return Out;
}


std::string printInterval(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2){
    std::ostringstream s;
    s << "[" << p1 << "," << p2 << "]";
    return s.str();
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