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
class Mod1Number;

struct begin_tag {};
struct end_tag {};

class SepSegmentCollection
{
public:


   // SepSegmentCollection(const SepSegmentCollection&) = delete;
    SepSegmentCollection& operator=(const SepSegmentCollection&) = delete;
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode, const begin_tag&);
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase, const SSCMode& sscMode, const end_tag&);

    std::list<SeparatrixSegment>::const_iterator operator[](std::size_t index) const { return m_segments[index]; }
    bool isEmpty() const { return m_segments.empty(); }
    void clear() { m_segments.clear(); }
    std::size_t size() const { return m_segments.size(); }
    std::vector<std::list<SeparatrixSegment>::const_iterator>::const_iterator begin() const { return m_segments.begin(); }
    std::vector<std::list<SeparatrixSegment>::const_iterator>::const_iterator end() const { return m_segments.end(); }


    void setInitialSetting(const Choose &sepIndicesChoose);

    void advance(std::size_t maxDepth, std::size_t maxInvolvedSingularities);

    friend bool operator==(const SepSegmentCollection& lhs, const SepSegmentCollection& rhs){
        return lhs.m_segments == rhs.m_segments;
    }
    //const SepSegmentIndex& operator[](std::size_t index) const { return *m_segments[index]; }

private:
    void setFirstSegments(const std::vector<SepSegmentIndex>& ssic);
    void setSegmentToFirst(std::size_t segmentIndex);


private:
    const SepSegmentDatabase& m_sepSegmentDatabase;
    std::vector<std::list<SeparatrixSegment>::const_iterator> m_segments;
    Choose m_sepIndicesChoose;
    const SSCMode& m_sscMode;
};


std::vector<Mod1Number> getEndpoints(const SepSegmentCollection& collection);


















}

#endif // SEPSEGMENTCOLLECTION_H