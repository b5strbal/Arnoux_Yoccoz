/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the Foliation class.
 */

#ifndef __Arnoux_Yoccoz__Foliation__
#define __Arnoux_Yoccoz__Foliation__

#include <vector>
#include "UnitIntervalPoint.h"
#include "IntervalExchangeMap.h"
#include "PerronFrobenius.h"
#include "FoliationRP2.h"
#include "FoliationSphere.h"



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
    friend class SeparatrixSegment;
    friend class ArcsAroundDivPoints;
    friend class SepSegmentDatabase;
    
public:
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    Foliation(const FoliationRP2&);
    Foliation(const FoliationSphere&);
    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;
    
   // int numSeparatrices() const { return m_numSeparatrices; }
   // int numIntervals() const { return m_numIntervals; }

    friend std::ostream& operator<<(std::ostream& Out, Foliation f);

protected:
//    typedef std::pair<UnitIntervalPoint, UnitIntervalPoint> interval_t;


// MEMBER VARIABLES
    int m_numIntervals;
    int m_numSeparatrices;
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<UnitIntervalPoint> m_allRealDivPoints;
    std::vector<short> m_isTopDivPoint;
    std::vector<UnitIntervalPoint> m_topRealDivPoints;
    std::vector<UnitIntervalPoint> m_bottomRealDivPoints;
    std::vector<int> m_pairOfTopDivPoints;

private:
    Foliation(const TwistedIntervalExchangeMap&);
    
    void init();
    
};


Foliation arnouxYoccozFoliation(int genus);








#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
