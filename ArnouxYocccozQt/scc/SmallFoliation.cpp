#include "SmallFoliation.h"
#include "TransverseCurve.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::SmallFoliation::SmallFoliation(const balazs::TransverseCurve &tc,
                                       std::size_t referenceZeroIndex, bool flippedOver, bool orientationReversing)
    : m_totalLength(tc.disjointIntervals().totalLength())
{
    assert(referenceZeroIndex < tc.foliation().numIntervals());

    bool movingForward = (!orientationReversing && !flippedOver) || (orientationReversing && flippedOver);
    const std::vector<Mod1NumberIntExchange>& bottomIntersections = movingForward ? tc.bottomRightIntersections() :
                                                                                    tc.bottomLeftIntersections();

    const std::vector<Mod1NumberIntExchange>& frontIntersections = flippedOver ? bottomIntersections :
                                                                                 tc.topIntersections();
    const std::vector<Mod1NumberIntExchange>& backIntersections = flippedOver ? tc.topIntersections() :
                                                                                 bottomIntersections;

    Mod1NumberIntExchange referenceZero = frontIntersections[referenceZeroIndex];



    // frontDivPoints
    std::vector<Mod1NumberIntExchange> frontDivPoints;
    frontDivPoints.reserve(tc.foliation().numIntervals());
    {
        Mod1NumberIntExchange distance;
        for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
            distance = movingForward ? tc.distanceOnCurve(referenceZero, frontIntersections[i]) :
                                       tc.distanceOnCurve(frontIntersections[i], referenceZero);
            frontDivPoints.push_back(distance);
        }
    }
    std::vector<Mod1NumberIntExchange> sortedFrontDivPoints = frontDivPoints;
    std::sort(sortedFrontDivPoints.begin(), sortedFrontDivPoints.end());



    // backDivPoints
    std::vector<Mod1NumberIntExchange> backDivPoints;
    backDivPoints.reserve(tc.foliation().numIntervals());
    {
        Mod1NumberIntExchange distance;
        for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
            distance = movingForward ? tc.distanceOnCurve(referenceZero, backIntersections[i]) :
                                       tc.distanceOnCurve(backIntersections[i], referenceZero);
            backDivPoints.push_back(distance);
        }
    }
    std::vector<Mod1NumberIntExchange> sortedBackDivPoints = frontDivPoints;
    std::sort(sortedBackDivPoints.begin(), sortedBackDivPoints.end());




    // permutation
    std::vector<std::size_t> permutationInput;
    for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
        std::size_t singularity = std::find(frontDivPoints.begin(), frontDivPoints.end(), sortedFrontDivPoints[i]) -
                frontDivPoints.begin();
        permutationInput[i] = std::lower_bound(sortedBackDivPoints.begin(), sortedBackDivPoints.end(), backDivPoints[singularity]) - sortedBackDivPoints.begin();
    }
    m_permutation = Permutation(permutationInput);



    // m_lengths
    m_lengths.reserve(tc.foliation().numIntervals());
    for(std::size_t i = 0; i < tc.foliation().numIntervals() - 1; i++){
        m_lengths.push_back(sortedFrontDivPoints[i + 1] - sortedFrontDivPoints[i]);
    }
    m_lengths.push_back(m_totalLength - sortedFrontDivPoints.back());




    // m_twist
    m_twist = sortedBackDivPoints[0];


    // m_normalizedLengths
    m_normalizedLengths.reserve(m_lengths.size());
    for(const auto& x : m_lengths){
        m_normalizedLengths.emplace_back(x / m_totalLength);
    }

    // m_normalizedTwist
    m_normalizedTwist = m_twist / m_totalLength;
}



