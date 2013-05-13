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

    for(std::size_t i = 1; i < numSeparatrices(); i++){
        if(distanceBetween(m_allDivPoints[i - 1], m_allDivPoints[i]) < PRECISION){
            throw std::runtime_error("The foliation has a saddle connection.");
        }
    }

    initSingularities();
}





balazs::Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    Foliation(TwistedIntervalExchangeMap(lengths, permutation, twist))
{
}

int balazs::Foliation::eulerChar() const
{
    int retval = 0;
    for(auto vec : m_singularities){
        retval -= vec.size() - 1;
    }
    return retval;
}

std::vector<std::size_t> balazs::Foliation::singularityTypeOfAbelianDiff() const
{
    std::vector<std::size_t> retval;
    for(auto vec : m_singularities){
        retval.push_back(vec.size() - 1);
    }
    std::sort(retval.begin(), retval.end(), std::greater<std::size_t>());
    return retval;
}




std::vector<std::size_t> balazs::Foliation::singularityTypeProngs() const
{
    std::vector<std::size_t> retval = singularityTypeOfAbelianDiff();
    for(auto &x : retval){
        x = (x + 1) * 2;
    }
    return retval;
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











balazs::Foliation balazs::Foliation::rotateBy(int rotationAmount) const{
    return Foliation(m_twistedIntervalExchange.rotateBy(rotationAmount));
}




balazs::Foliation balazs::Foliation::reflect() const{
    return Foliation(m_twistedIntervalExchange.reverse());
}




balazs::Foliation balazs::Foliation::flipOver() const{
    return Foliation(m_twistedIntervalExchange.invert().reverse());
}



void balazs::Foliation::initSingularities()
{
    std::size_t size = m_twistedIntervalExchange.size();
    m_indexOfSingularity.resize(size);

    std::vector<char> alreadyLookedAt(size, false);
    for(std::size_t i = 0; i < size; i++){
        if(!alreadyLookedAt[i]){
            m_singularities.push_back(std::vector<std::size_t>(0));

            std::size_t j = i;
            do {
                m_singularities.back().push_back(j);
                m_indexOfSingularity[j] = m_singularities.size() - 1;
                alreadyLookedAt[j] = true;


                j = integerMod(j + size - 1, size);
                j = m_twistedIntervalExchange.permutationWithMinimalTwist()[j];
                j = integerMod(j + 1, size);
                j = m_twistedIntervalExchange.inversePermutationWithMinimalTwist()[j];
            } while(j != i);
        }
    }
}






std::ostream& balazs::operator<<(std::ostream& out, const Foliation& f){
    out << f.m_twistedIntervalExchange;
    return out;
}






balazs::Foliation balazs::arnouxYoccozFoliation(int genus){
    std::vector<std::size_t> permutationInput(2 * genus);
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



