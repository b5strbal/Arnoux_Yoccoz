#include "SmallFoliation.h"
#include "TransverseCurve.h"
#include "SeparatrixSegment.h"
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
    std::vector<Mod1NumberIntExchange> sortedBackDivPoints = backDivPoints;
    std::sort(sortedBackDivPoints.begin(), sortedBackDivPoints.end());




    // permutation, strip heights
    for(HDirection hDirection : { HDirection::Left, HDirection::Right}){
        m_stripHeights[hDirection].resize(tc.foliation().numIntervals());
        for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
            m_stripHeights[hDirection][i].resize(tc.foliation().numIntervals());
        }
    }

    std::vector<std::size_t> permutationInput;
    for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
        std::size_t singularity = std::find(frontDivPoints.begin(), frontDivPoints.end(), sortedFrontDivPoints[i]) -
                frontDivPoints.begin();
        permutationInput.push_back(std::lower_bound(sortedBackDivPoints.begin(), sortedBackDivPoints.end(),
                                               backDivPoints[singularity]) - sortedBackDivPoints.begin());

        for(std::size_t j = 0; j < tc.foliation().numIntervals(); j++){
            std::map<HDirection, HDirection> source;
            source[HDirection::Right] = movingForward ? HDirection::Right : HDirection::Left;
            source[HDirection::Left] = movingForward ? HDirection::Left : HDirection::Right;

            for(HDirection hDirection : { HDirection::Left, HDirection::Right}){

            const SeparatrixSegment& upCentered = tc.touchingSepSegment({source[hDirection], VDirection::Up, singularity},SepSegmentDatabase::Centered);
            const SeparatrixSegment& downCentered = tc.touchingSepSegment({source[hDirection], VDirection::Down, singularity},SepSegmentDatabase::Centered);
            const SeparatrixSegment& upShifted = tc.touchingSepSegment({source[hDirection], VDirection::Up, singularity},SepSegmentDatabase::ShiftedEvenMore);
            const SeparatrixSegment& downShifted = tc.touchingSepSegment({source[hDirection], VDirection::Down, singularity},SepSegmentDatabase::ShiftedEvenMore);

            if(upCentered.depth() < upShifted.depth()){
                m_stripHeights[hDirection][i][j] = downCentered.intervalIntersectionCount()[j] -
                        downShifted.intervalIntersectionCount()[j];
            } else

            if(downCentered.depth() < downShifted.depth()){
                m_stripHeights[hDirection][i][j] = upCentered.intervalIntersectionCount()[j] -
                        upShifted.intervalIntersectionCount()[j];
            } else {
                m_stripHeights[hDirection][i][j] = upShifted.intervalIntersectionCount()[j] +
                        downShifted.intervalIntersectionCount()[j];
            }

            }

        }



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



void balazs::SmallFoliation::initStripHeights()
{

}



