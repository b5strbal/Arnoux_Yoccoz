#ifndef SEPSEGMENTCOLLECTION_H
#define SEPSEGMENTCOLLECTION_H

#include "../math/Choose.h"
#include "SepSegmentDatabase.h"
#include "SSCMode.h"

namespace balazs{







class SepSegmentCollection
{
public:
    static SepSegmentCollection emptyCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);
    static SepSegmentCollection firstCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);

    sepSegmentDatabaseConstIterator_t operator[](std::size_t index) const { return m_segments[index]; }
    bool isEmpty() const { return m_segments.empty(); }
    void clear() { m_segments.clear(); }
    std::size_t size() const { return m_segments.size(); }
    std::vector<sepSegmentDatabaseConstIterator_t>::iterator begin() { return m_segments.begin(); }
    std::vector<sepSegmentDatabaseConstIterator_t>::iterator end() { return m_segments.end(); }


    void setInitialSetting(const Choose &sepIndicesChoose){
        m_sepIndicesChoose = sepIndicesChoose;
        setFirstSegments(m_sscMode.initialSegments(sepIndicesChoose));
    }

    void advance(std::size_t maxDepth, std::size_t maxInvolvedSingularities);

    friend bool operator==(const SepSegmentCollection& lhs, const SepSegmentCollection& rhs){
        return lhs.m_segments == rhs.m_segments;
    }


private:
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);
    const Foliation& foliation() const { return m_sepSegmentDatabase.foliation(); }
    void setFirstSegments(const std::vector<SepSegmentIndex>& ssic);
    void setSegmentToFirst(std::size_t segmentIndex){
        m_segments[segmentIndex] = m_sepSegmentDatabase.firstGoodSegment(getSepSegmentIndex(m_segments[segmentIndex]));
    }


private:
    const SepSegmentDatabase& m_sepSegmentDatabase;
    std::vector<sepSegmentDatabaseConstIterator_t> m_segments;
    Choose m_sepIndicesChoose;
    const SSCMode& m_sscMode;
};











inline SepSegmentCollection SepSegmentCollection::emptyCollection(const SepSegmentDatabase& sepSegmentDatabase,
                                            const SSCMode& sscMode) {
    return SepSegmentCollection(sepSegmentDatabase, sscMode);
}

inline SepSegmentCollection SepSegmentCollection::firstCollection(const SepSegmentDatabase& sepSegmentDatabase,
                                            const SSCMode& sscMode) {
    SepSegmentCollection retVal(sepSegmentDatabase, sscMode);
    retVal.setInitialSetting(Choose(retVal.m_sscMode.howMuchToChooseFrom(), 1));
    return retVal;
}







}

#endif // SEPSEGMENTCOLLECTION_H
