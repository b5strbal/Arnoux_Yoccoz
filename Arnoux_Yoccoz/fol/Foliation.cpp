/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"
#include "../math/Modint.h"




balazs::Foliation::Foliation(const TwistedIntervalExchangeMap& twistedintervalExchange) :
    m_twistedIntervalExchange(twistedintervalExchange)
{
    m_allDivPoints.reserve(numSeparatrices());
    std::merge(topDivPoints().begin(), topDivPoints().end(),
               bottomDivPoints().begin(), bottomDivPoints().end(), std::back_inserter(m_allDivPoints));

    for(unsigned int i = 1; i < numSeparatrices(); i++){
        if(distanceBetween(m_allDivPoints[i - 1], m_allDivPoints[i]) < PRECISION){
            throw std::runtime_error("The foliation has a saddle connection.");
        }
    }
}





balazs::Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    Foliation(TwistedIntervalExchangeMap(lengths, permutation, twist))
{
}

balazs::Foliation balazs::Foliation::fromFoliationRP2(const FoliationRP2 &foliationRP2)
{
    return fromFoliationSphere(FoliationSphere(foliationRP2));
}

balazs::Foliation balazs::Foliation::fromFoliationSphere(const FoliationSphere &foliationSphere)
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
        lengths[i] = distanceBetween( allConnectedPointsSortedByTop[i].topPoint, allConnectedPointsSortedByTop[i + 1].topPoint);
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






void balazs::Foliation::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
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



void balazs::Foliation::generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                                            std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.bottomFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] != Modint(i - 1, numSeparatrices) ) {

            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = static_cast<Mod1Number>(foliationSphere.bottomFoliation().intervalPairing().divPoints()[i]) + foliationSphere.twist();

            int indexOfConnectedSeparatrix = Modint(foliationSphere.bottomFoliation().intervalPairing().permutation()[i] + 1, numSeparatrices);
            Mod1Number middlePoint = static_cast<Mod1Number>(foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix]) + foliationSphere.twist();

            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().applyTo(middlePoint);

            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}





bool balazs::Foliation::isTopDivPoint(int divPointIndex) const{
    return m_allDivPoints[divPointIndex].twistCoeff() == 0;
}

const balazs::Mod1NumberIntExchange &balazs::Foliation::firstIntersection(int singularityIndex, Direction::UpOrDown direction) const
{
    if (direction == Direction::UP) {
        return bottomDivPoints()[m_twistedIntervalExchange.permutationWithMinimalTwist()[singularityIndex]];
    } else
        return topDivPoints()[singularityIndex];
}

unsigned int balazs::Foliation::smallContainingInterval(const Mod1Number &point) const
{
    return containingInterval(m_allDivPoints, point);
}


balazs::Foliation balazs::Foliation::rotateBy(int rotationAmount) const{
    return Foliation(m_twistedIntervalExchange.rotateBy(rotationAmount));
}


balazs::Foliation balazs::Foliation::reflect() const{
    return Foliation(m_twistedIntervalExchange.reverse());
}


balazs::Foliation balazs::Foliation::flipOver() const{
    return Foliation(m_twistedIntervalExchange.invert().reverse());
}






std::ostream& balazs::operator<<(std::ostream& out, const Foliation& f){
    out << f.m_twistedIntervalExchange;
    return out;
}






balazs::Foliation balazs::arnouxYoccozFoliation(int genus){
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

















