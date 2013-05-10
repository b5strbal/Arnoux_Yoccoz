#include "SSCMode.h"




std::vector<balazs::SepSegmentIndex> balazs::SSCModeShiftToSameSide::initialSegments(const Choose& sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(2 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        retVal.push_back({ m_shiftToSide, Direction::UP, sepIndicesChoose[i] });
        retVal.push_back({ m_shiftToSide, Direction::DOWN, sepIndicesChoose[i] });
    }
    return retVal;
}




//--------------------------------------//--------------------------------------//--------------------------------------



std::vector<balazs::SepSegmentIndex> balazs::SSCModeSingWrap::initialSegments(const balazs::Choose &sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(2 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        retVal.push_back({ Direction::LEFT, Direction::UP, sepIndicesChoose[i] });
        retVal.push_back({ Direction::RIGHT, Direction::UP, sepIndicesChoose[i] });
    }
    return retVal;

}














//--------------------------------------//--------------------------------------//-------------------------------------







balazs::SSCModeShiftToSameSideFromRP2::SSCModeShiftToSameSideFromRP2(const balazs::Foliation &foliation, balazs::Direction::LeftOrRight shiftToSide)
    : SSCMode(foliation),
      m_shiftToSide(shiftToSide),
      m_foliationFromRP2(dynamic_cast<const FoliationFromRP2&>(this->foliation()))
{
    m_choiceOfSingularities_RP2.reserve(m_foliationFromRP2.numIntervals() / 2);
    for(std::size_t i = 0; i < m_foliationFromRP2.numIntervals(); i++){
        if(m_foliationFromRP2.intervalPermutationBeforeHalfTwist()[i] > i){
            m_choiceOfSingularities_RP2.push_back(i);
        }
    }

}




std::vector<balazs::SepSegmentIndex> balazs::SSCModeShiftToSameSideFromRP2::initialSegments(const balazs::Choose &sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(4 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        std::size_t firstIndex = m_choiceOfSingularities_RP2[sepIndicesChoose[i]];
        std::size_t secondIndex = m_foliationFromRP2.intervalPermutationBeforeHalfTwist()[firstIndex];

        retVal.push_back({ m_shiftToSide, Direction::UP, firstIndex });
        retVal.push_back({ m_shiftToSide, Direction::DOWN, firstIndex });
        retVal.push_back({ m_shiftToSide, Direction::UP, secondIndex });
        retVal.push_back({ m_shiftToSide, Direction::DOWN, secondIndex });
    }
    return retVal;
}


std::vector<std::size_t> balazs::SSCModeShiftToSameSideFromRP2::additionalSegmentsToSetToFirst(std::size_t indexToIncrease) const{
    switch (indexToIncrease % 4) {
    case 2:
        return {{ indexToIncrease - 2 }};
    case 3:
        return {{ }};
    default:
        assert(false);
    }
}


std::vector<std::size_t> balazs::SSCModeShiftToSameSideFromRP2::segmentsToLengthen(std::size_t indexToIncrease) const{
    switch (indexToIncrease % 4) {
    case 2:
        return {{ indexToIncrease - 1, indexToIncrease }};
    case 3:
        return {{ indexToIncrease - 3, indexToIncrease }};
    default:
        assert(false);
    }
}

