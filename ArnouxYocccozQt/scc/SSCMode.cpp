#include "SSCMode.h"
#include "SepSegmentDatabase.h"
#include "../math/Choose.h"
#include "../fol/Foliation.h"
#include "../fol/FoliationFromRP2.h"
#include <cassert>

balazs::SSCModeShiftToSameSide::SSCModeShiftToSameSide(const Foliation& foliation, HDirection shiftToSide)
    : SSCMode(foliation), m_shiftToSide(shiftToSide)
{
    assert(shiftToSide != HDirection::Center);
}


std::vector<balazs::SepSegmentIndex> balazs::SSCModeShiftToSameSide::initialSegments(const Choose& sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(2 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        retVal.push_back({ m_shiftToSide, VDirection::Up, sepIndicesChoose[i] });
        retVal.push_back({ m_shiftToSide, VDirection::Down, sepIndicesChoose[i] });
    }
    return retVal;
}

std::size_t balazs::SSCModeShiftToSameSide::numInvolvedSingularities(const Choose &sepIndicesChoose) const
{
    return sepIndicesChoose.k();
}

std::size_t balazs::SSCModeShiftToSameSide::howMuchToChooseFrom() const
{
    return foliation().numIntervals();
}




//--------------------------------------//--------------------------------------//--------------------------------------



std::vector<balazs::SepSegmentIndex> balazs::SSCModeSingWrap::initialSegments(const balazs::Choose &sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(2 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        retVal.push_back({ HDirection::Left, VDirection::Up, sepIndicesChoose[i] });
        retVal.push_back({ HDirection::Right, VDirection::Up, sepIndicesChoose[i] });
    }
    return retVal;

}


std::size_t balazs::SSCModeSingWrap::numInvolvedSingularities(const Choose &sepIndicesChoose) const
{
    return sepIndicesChoose.k();
}


std::size_t balazs::SSCModeSingWrap::howMuchToChooseFrom() const
{
    return foliation().numIntervals();
}


std::vector<std::size_t> balazs::SSCModeSingWrap::segmentsToLengthen(std::size_t indexToIncrease) const {
    assert(indexToIncrease % 2 == 1);
    return {{ indexToIncrease-1, indexToIncrease }};
}









//--------------------------------------//--------------------------------------//-------------------------------------







balazs::SSCModeShiftToSameSideFromRP2::SSCModeShiftToSameSideFromRP2(const Foliation &foliation, HDirection shiftToSide)
    : SSCMode(foliation),
      m_shiftToSide(shiftToSide),
      m_foliationFromRP2(dynamic_cast<const FoliationFromRP2&>(foliation))
{
    assert(shiftToSide != HDirection::Center);
    m_choiceOfSingularities_RP2.reserve(m_foliationFromRP2.numIntervals() / 2);
    for(std::size_t i = 0; i < m_foliationFromRP2.numIntervals(); i++){
        if(m_foliationFromRP2.intervalPermutationBeforeHalfTwist()[i] > i){
            m_choiceOfSingularities_RP2.push_back(i);
        }
    }

}



std::size_t balazs::SSCModeShiftToSameSideFromRP2::numInvolvedSingularities(const Choose &sepIndicesChoose) const
{
    return 2 * sepIndicesChoose.k();
}


std::size_t balazs::SSCModeShiftToSameSideFromRP2::howMuchToChooseFrom() const
{
    return foliation().numIntervals() / 2;
}



std::vector<balazs::SepSegmentIndex> balazs::SSCModeShiftToSameSideFromRP2::initialSegments(const balazs::Choose &sepIndicesChoose) const
{
    std::vector<SepSegmentIndex> retVal;

    retVal.reserve(4 * sepIndicesChoose.k());
    for(std::size_t i = 0; i < sepIndicesChoose.k(); i++){
        std::size_t firstIndex = m_choiceOfSingularities_RP2[sepIndicesChoose[i]];
        std::size_t secondIndex = m_foliationFromRP2.intervalPermutationBeforeHalfTwist()[firstIndex];

        retVal.push_back({ m_shiftToSide, VDirection::Up, firstIndex });
        retVal.push_back({ m_shiftToSide, VDirection::Down, firstIndex });
        retVal.push_back({ m_shiftToSide, VDirection::Up, secondIndex });
        retVal.push_back({ m_shiftToSide, VDirection::Down, secondIndex });
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

