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


//-------------------------//
// InitArguments_Foliation //
//-------------------------//


class InitArguments_Foliation{
    friend class Foliation;
    
    struct ConnectedPoints {
        UnitIntervalPoint topPoint;
        UnitIntervalPoint bottomPoint;
    };
    
    InitArguments_Foliation() {}
    InitArguments_Foliation(const FoliationSphere& foliationSphere);
    void generateTopConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    void generateBottomConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    
    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
    static floating_point_type arg_twist;
};




//-----------//
// Foliation //
//-----------//


class Foliation : private InitArguments_Foliation{
    
public:
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    Foliation(const FoliationRP2&);
    Foliation(const FoliationSphere&);
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





//------------------//
// FoliationFromRP2 //
//------------------//


class FoliationFromRP2 : public Foliation
{
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);
};









//---------------------//
// FoliationFromSphere //
//---------------------//

class FoliationFromSphere : public Foliation
{
public:
    FoliationFromSphere(const FoliationSphere& foliationSphere);
};



#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
