/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"
#include "../math/PerronFrobenius.h"



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




const std::vector<balazs::Mod1NumberIntExchange>& balazs::Foliation::topDivPoints() const {
    return m_twistedIntervalExchange.divPoints();
}




const std::vector<balazs::Mod1NumberIntExchange>& balazs::Foliation::bottomDivPoints() const {
    return m_twistedIntervalExchange.divPointsAfterExchange();
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

















