#ifndef SEPSEGMENTCOLLECTION_H
#define SEPSEGMENTCOLLECTION_H

#include <list>
#include "../math/Choose.h"


namespace balazs{


class SepSegmentDatabase;
class SSCMode;
struct SepSegmentIndex;
class SeparatrixSegment;
class Foliation;

class SepSegmentCollection
{
public:
    static SepSegmentCollection emptyCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);
    static SepSegmentCollection firstCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);

    std::list<SeparatrixSegment>::const_iterator operator[](std::size_t index) const { return m_segments[index]; }
    bool isEmpty() const { return m_segments.empty(); }
    void clear() { m_segments.clear(); }
    std::size_t size() const { return m_segments.size(); }
    std::vector<std::list<SeparatrixSegment>::const_iterator>::iterator begin() { return m_segments.begin(); }
    std::vector<std::list<SeparatrixSegment>::const_iterator>::iterator end() { return m_segments.end(); }


    void setInitialSetting(const Choose &sepIndicesChoose);

    void advance(std::size_t maxDepth, std::size_t maxInvolvedSingularities);

    friend bool operator==(const SepSegmentCollection& lhs, const SepSegmentCollection& rhs){
        return lhs.m_segments == rhs.m_segments;
    }


private:
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode);
    void setFirstSegments(const std::vector<SepSegmentIndex>& ssic);
    void setSegmentToFirst(std::size_t segmentIndex);


private:
    const SepSegmentDatabase& m_sepSegmentDatabase;
    std::vector<std::list<SeparatrixSegment>::const_iterator> m_segments;
    Choose m_sepIndicesChoose;
    const SSCMode& m_sscMode;
};











inline SepSegmentCollection SepSegmentCollection::emptyCollection(const SepSegmentDatabase& sepSegmentDatabase,
                                            const SSCMode& sscMode) {
    return SepSegmentCollection(sepSegmentDatabase, sscMode);
}








}

#endif // SEPSEGMENTCOLLECTION_H
