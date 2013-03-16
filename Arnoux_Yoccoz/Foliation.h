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
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    
    
private:
    int m_numSeparatrices;
    std::vector<UnitIntervalPoint> m_shiftedSeparatrixPoints;
    TwistedIntervalExchangeMap m_intervalExchange;
    
    
    class ArcsAroundDivPoints;
    
    /**
     * @brief   Takes the intersection of two ArcsAroundDivPoints objects.
     */
    ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2);
};










class FoliationFromRP2 : public Foliation{
public:
    FoliationFromRP2();




};





#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
