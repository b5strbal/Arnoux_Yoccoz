/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"
#include "Modint.h"




//-----------//
// Foliation //
//-----------//

Foliation::Foliation(const TwistedIntervalExchangeMap& twistedintervalExchange) :
    m_twistedIntervalExchange(twistedintervalExchange)
{
    init();
}


Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    Foliation(TwistedIntervalExchangeMap(lengths, permutation, twist))
{
}

Foliation Foliation::fromFoliationRP2(const FoliationRP2 &foliationRP2)
{
    return fromFoliationSphere(FoliationSphere(foliationRP2));
}

Foliation Foliation::fromFoliationSphere(const FoliationSphere &foliationSphere)
{

    std::vector<ConnectedPoints> allConnectedPoints;
    allConnectedPoints.reserve(foliationSphere.topFoliation().numSeparatrices() + foliationSphere.bottomFoliation().numSeparatrices());

    generateTopConnectingPairs(foliationSphere, allConnectedPoints);
    generateBottomConnectingPairs(foliationSphere, allConnectedPoints);


    std::sort(allConnectedPoints.begin(), allConnectedPoints.end(), [] (const ConnectedPoints& cp1, const ConnectedPoints& cp2)
              {
                  return cp1.topPoint < cp2.topPoint;
              } );
    std::vector<ConnectedPoints>& allConnectedPointsSortedByTop = allConnectedPoints;
    std::vector<Mod1Number> allBottomPoints;
    allBottomPoints.reserve(allConnectedPoints.size());

    for (ConnectedPoints cp : allConnectedPoints) {
        allBottomPoints.push_back(cp.bottomPoint);
    }

    std::sort(allBottomPoints.begin(), allBottomPoints.end());

    std::vector<floating_point_type> lengths(allConnectedPoints.size());
    for (unsigned int i = 0; i < allConnectedPoints.size() - 1; i++) {
        lengths[i] = Mod1Number::distanceBetween( allConnectedPointsSortedByTop[i].topPoint, allConnectedPointsSortedByTop[i + 1].topPoint);
    }
    lengths[allConnectedPoints.size() - 1] = -allConnectedPointsSortedByTop[allConnectedPoints.size() - 1].topPoint;

    std::vector<unsigned int> permutationInput(allConnectedPoints.size());
    for (unsigned int i = 0; i < allConnectedPoints.size(); i++) {
        auto it = std::lower_bound(allBottomPoints.begin(), allBottomPoints.end(), allConnectedPointsSortedByTop[i].bottomPoint);
        permutationInput[i] = it - allBottomPoints.begin() ;
    }
    Permutation permutation(permutationInput);
    floating_point_type twist = allBottomPoints[0];

    return Foliation(lengths, permutation, twist);
}








void Foliation::init(){
    m_numIntervals = m_twistedIntervalExchange.size();
    m_numSeparatrices = 2 * m_numIntervals;
    
    struct DivPoint{
        Mod1Number m_point;
        bool m_isTopPoint;
    };
    
    std::vector<DivPoint> divPoints;
    divPoints.reserve(m_numSeparatrices);
    
    m_topRealDivPoints.reserve(m_numIntervals);
    m_pairOfTopDivPoints.reserve(m_numIntervals);
    for (int i = 0; i < m_numIntervals + 1; i++) {
        if (i != m_twistedIntervalExchange.m_indexOfFakeDivPoint) {
            m_topRealDivPoints.push_back(m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPoints()[i]);
            m_pairOfTopDivPoints.push_back(m_twistedIntervalExchange.m_intervalExchangeAfterTwist.permutation()[i] - 1);

            DivPoint divpoint;
            divpoint.m_point = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPoints()[i];
            divpoint.m_isTopPoint = true;
            divPoints.push_back(divpoint);
        }
    }
    
    m_bottomRealDivPoints.reserve(m_numIntervals);
    auto temp = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPointsAfterExchange();
    for(auto it = temp.begin() + 1; it != temp.end(); it++)
        m_bottomRealDivPoints.push_back(*it);
    
    
    for (int i = 1; i < m_numIntervals + 1; i++) {
        DivPoint divpoint;
        divpoint.m_point = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPointsAfterExchange()[i];
        divpoint.m_isTopPoint = false;
        divPoints.push_back(divpoint);
    }
    
    std::inplace_merge(divPoints.begin(), divPoints.begin() + m_numIntervals, divPoints.end(), [] (const DivPoint& p1, const DivPoint& p2)
                       {
                           return p1.m_point < p2.m_point;
                       } );
    
    m_allRealDivPoints.reserve(m_numSeparatrices);
    m_isTopDivPoint.reserve(m_numSeparatrices);
    for (auto p : divPoints) {
        m_allRealDivPoints.push_back(p.m_point);
        m_isTopDivPoint.push_back(p.m_isTopPoint);
    }
    
    assert(!arePointsTooClose(m_allRealDivPoints));
  /*
    std::cout << m_isTopDivPoint << std::endl;
    std::cout << m_allRealDivPoints << std::endl;
    std::cout << m_topRealDivPoints << std::endl;
    std::cout << m_bottomRealDivPoints << std::endl;
    std::cout << m_pairOfTopDivPoints << std::endl;
*/
    

}



void Foliation::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                                         std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.topFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        Modint modi(i, numSeparatrices);
        if (foliationSphere.topFoliation().intervalPairing().permutation()[modi] != Modint(i - 1, numSeparatrices)) {
            // otherwise the current separatrix emanates from a 1-pronged singularity which is not important

            ConnectedPoints newConnectedPoints;
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().divPoints()[i];

            int indexOfConnectedSeparatrix = Modint(foliationSphere.topFoliation().intervalPairing().permutation()[i]+ 1, numSeparatrices);
            Mod1Number middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix];

            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().applyTo(middlePoint - foliationSphere.twist()) + foliationSphere.twist();


            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}



void Foliation::generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                                            std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.bottomFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] != Modint(i - 1, numSeparatrices) ) {

            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().divPoints()[i] + foliationSphere.twist();

            int indexOfConnectedSeparatrix = Modint(foliationSphere.bottomFoliation().intervalPairing().permutation()[i] + 1, numSeparatrices);
            Mod1Number middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix] + foliationSphere.twist();

            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().applyTo(middlePoint);

            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}








Foliation Foliation::rotateBy(int rotationAmount) const{
    return Foliation(m_twistedIntervalExchange.rotateBy(rotationAmount));
}


Foliation Foliation::reflect() const{
    return Foliation(m_twistedIntervalExchange.reflect());
}


Foliation Foliation::flipOver() const{
    return Foliation(m_twistedIntervalExchange.invert().reflect());
}






std::ostream& operator<<(std::ostream& out, const Foliation& f){
    out << f.m_twistedIntervalExchange;
    return out;
}






Foliation arnouxYoccozFoliation(int genus){
    std::vector<unsigned int> permutationInput(2 * genus);
    std::vector<floating_point_type> lengths(2 * genus);
    floating_point_type shrinkingNumber = 1/arnouxYoccozStretchFactor(genus);
    floating_point_type currentLength = shrinkingNumber;
    for (int i = 0; i < genus; i++) {
        permutationInput[2 * i] = 2 * i + 1;
        permutationInput[2 * i + 1] = 2 * i;
        lengths[2 * i] = lengths[2 * i + 1] = currentLength;
        currentLength *= shrinkingNumber;
    }
    try {
        return Foliation(lengths, Permutation(permutationInput), 0.5);
    } catch (const std::exception& ex) {
        throw std::runtime_error("Machine precision is not enough to handle such a high genus Arnoux-Yoccox surface.");
    }
}

















