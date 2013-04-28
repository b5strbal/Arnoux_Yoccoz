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
#include "../intex/TwistedIntervalExchangeMap.h"
#include "../math/PerronFrobenius.h"
#include "FoliationRP2.h"
#include "FoliationSphere.h"


namespace balazs{



class Foliation{
    friend class SeparatrixSegment;
    friend class IntervalNeighborhoods;
public:
    Foliation(const std::vector<floating_point_type>& lengths,
              const Permutation& permutation,
              floating_point_type twist);
    static Foliation fromFoliationRP2(const FoliationRP2 &foliationRP2);
    static Foliation fromFoliationSphere(const FoliationSphere&foliationSphere);
    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;
    
    unsigned int numSeparatrices() const { return 2 * numIntervals(); }
    unsigned int numIntervals() const { return m_twistedIntervalExchange.size(); }

    friend std::ostream& operator<<(std::ostream& out, const Foliation& f);

protected:
//    typedef std::pair<UnitIntervalPoint, UnitIntervalPoint> interval_t;

    const std::vector<Mod1NumberIntExchange>& topDivPoints() const { return m_twistedIntervalExchange.divPoints(); }
    const std::vector<Mod1NumberIntExchange>& bottomDivPoints() const { return m_twistedIntervalExchange.divPointsAfterExchange(); }
    bool isTopDivPoint(int divPointIndex) const;
    const Mod1NumberIntExchange& firstIntersection(int singularityIndex, Direction::UpOrDown direction) const;
    unsigned int smallContainingInterval(const Mod1Number& point) const;

// MEMBER VARIABLES
   // int m_numIntervals;
   // int m_numSeparatrices;
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<Mod1NumberIntExchange> m_allDivPoints;
    std::vector<short> m_isTopDivPoint;
   // std::vector<Mod1Number> m_topRealDivPoints;
   // std::vector<Mod1Number> m_bottomRealDivPoints;
    std::vector<int> m_pairOfTopDivPoints;

private:
    Foliation(const TwistedIntervalExchangeMap&);
    
    struct ConnectedPoints {
        Mod1Number topPoint;
        Mod1Number bottomPoint;
    };

    static void generateTopConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    static void generateBottomConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);

    
};


Foliation arnouxYoccozFoliation(int genus);


}





#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
