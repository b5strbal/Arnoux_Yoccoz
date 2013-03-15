/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the Foliation class.
 */

#ifndef __Arnoux_Yoccoz__Foliation__
#define __Arnoux_Yoccoz__Foliation__

#include <iostream>
#include <vector>
#include "UnitIntervalPoint.h"
#include "IntervalExchangeMap.h"


class Foliation{
public:
    Foliation(const std::vector<floating_point_type> lengths){
    
    }
    
    
    //int getNumDivPoints() const { return m_NumIntervals; }
    
private:
    int m_numIntervals;
    int m_numDivPoints;
    std::vector<floating_point_type> m_lengths;
    std::vector<floating_point_type> m_translations;
    std::vector<UnitIntervalPoint> m_divPoints;
    std::vector<UnitIntervalPoint> m_shiftedDivPoints;
    
    
    
    
    class ArcsAroundDivPoints;
    
    /**
     * @brief   Takes the intersection of two ArcsAroundDivPoints objects.
     */
    ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2);
};






#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
