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
#include "PerronFrobenius.h"
#include "FoliationRP2.h"


class Foliation{
    
public:
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;
    
    friend std::ostream& operator<<(std::ostream& Out, Foliation f);
    
private:
    Foliation(const TwistedIntervalExchangeMap&);
    
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    int m_numSeparatrices;

    
    
    class ArcsAroundDivPoints;
    
    /**
     * @brief   Takes the intersection of two ArcsAroundDivPoints objects.
     */
    ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2);
};


Foliation arnouxYoccozFoliation(int genus);








class InitArguments_FoliationFromRP2{
    friend class FoliationFromRP2;

    InitArguments_FoliationFromRP2(const FoliationRP2& foliationRP2);
    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
};




class FoliationFromRP2 : private InitArguments_FoliationFromRP2,
                         public Foliation
{
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);


};









class InitArguments_FoliationFromSphere{
    friend class FoliationFromSphere;
    
    InitArguments_FoliationFromSphere(const FoliationSphere& foliationSphere);
    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
    static floating_point_type arg_twist;
};




class FoliationFromSphere : private InitArguments_FoliationFromSphere,
                            public Foliation
{
public:
    FoliationFromSphere(const FoliationSphere& foliationSphere);


};



#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
