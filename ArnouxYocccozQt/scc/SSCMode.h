#ifndef SSCMODE_H
#define SSCMODE_H

#include <vector>
#include "../intex/HDirection.h"

namespace balazs {

class Choose;
class SepSegmentDatabase;
class Foliation;
class FoliationFromRP2;
struct SepSegmentIndex;



class SSCMode{
public:
    SSCMode(const Foliation& foliation) : m_foliation(foliation) {}
    const Foliation& foliation() const { return m_foliation; }
    virtual std::vector<SepSegmentIndex> initialSegments(const Choose &sepIndicesChoose) const = 0;
    virtual std::vector<std::size_t> segmentsToLengthen(std::size_t indexToIncrease) const = 0;
    virtual std::vector<std::size_t> additionalSegmentsToSetToFirst(std::size_t indexToIncrease) const = 0;
    virtual std::size_t numInvolvedSingularities(const Choose &sepIndicesChoose) const = 0;
    virtual std::size_t howMuchToChooseFrom() const = 0;
    virtual ~SSCMode() {}
private:
    const Foliation& m_foliation;
};






class SSCModeShiftToSameSide : public SSCMode
{
public:
    SSCModeShiftToSameSide(const Foliation& foliation, HDirection shiftToSide)
        : SSCMode(foliation), m_shiftToSide(shiftToSide) {}
    std::vector<SepSegmentIndex> initialSegments(const Choose &sepIndicesChoose) const;
    std::vector<std::size_t> segmentsToLengthen(std::size_t indexToIncrease) const { return {{ indexToIncrease }}; }
    std::vector<std::size_t> additionalSegmentsToSetToFirst(std::size_t indexToIncrease) const
        { (void)indexToIncrease; return {{}}; }
    std::size_t numInvolvedSingularities(const Choose &sepIndicesChoose) const;
    std::size_t howMuchToChooseFrom() const;
private:
    HDirection m_shiftToSide;
};








class SSCModeSingWrap : public SSCMode
{
public:
    SSCModeSingWrap(const Foliation& foliation) : SSCMode(foliation) {}
    std::vector<SepSegmentIndex> initialSegments(const Choose &sepIndicesChoose) const;
    std::vector<std::size_t> segmentsToLengthen(std::size_t indexToIncrease) const;
    std::vector<std::size_t> additionalSegmentsToSetToFirst(std::size_t indexToIncrease) const
        { (void)indexToIncrease; return {{}}; }
    std::size_t numInvolvedSingularities(const Choose &sepIndicesChoose) const;
    std::size_t howMuchToChooseFrom() const;
};









class SSCModeShiftToSameSideFromRP2 : public SSCMode
{
public:
    SSCModeShiftToSameSideFromRP2(const Foliation& foliation, HDirection shiftToSide);
    std::vector<SepSegmentIndex> initialSegments(const Choose &sepIndicesChoose) const;
    std::vector<std::size_t> segmentsToLengthen(std::size_t indexToIncrease) const;
    std::vector<std::size_t> additionalSegmentsToSetToFirst(std::size_t indexToIncrease) const;
    std::size_t numInvolvedSingularities(const Choose &sepIndicesChoose) const;
    std::size_t howMuchToChooseFrom() const;
private:
    HDirection m_shiftToSide;
    const FoliationFromRP2& m_foliationFromRP2;

    // This vector is only filled in if the underlying foliation is a FoliationFromRP2.
    // In that case the singularities/separatrices are naturally paired up by the covering map.
    // That pairing is described by FoliationFromRP2::intervalPermutationBeforeHalfTwist().
    // This vector contains the smaller indices of the singularities in each pair.
    // So its length is half of numIntervals of the foliation.
    std::vector<std::size_t> m_choiceOfSingularities_RP2;
};





}



#endif // SSCMODE_H
