#ifndef SEPARATRIXSEGMENT_H
#define SEPARATRIXSEGMENT_H

#include "IntervalNeighborhoods.h"
#include "../fol/VDirection.h"
#include "../intex/HDirection.h"

namespace balazs{

struct SepSegmentIndex{
    HDirection hDirection;
    VDirection vDirection;
    std::size_t singularityIndex;
};

class SeparatrixSegment{
public:
    SeparatrixSegment(const Foliation& foliation, const SepSegmentIndex& ssIndex);
 //   SeparatrixSegment(const SeparatrixSegment&) = delete;
    SeparatrixSegment& operator=(const SeparatrixSegment&) = delete;

    bool reachedSaddleConnection() const { return m_reachedSaddleConnection; }
    void lengthen();
    bool isGood() const;

    const Foliation& foliation() const { return m_foliation; }
    HDirection hDirection() const { return m_endpoint.number().side(); }
    std::size_t depth() const { return m_depth; }
    const Mod1NumberIntExWithInfo& endpoint() const { return m_endpoint; }
    std::size_t startingSingularity() const { return m_startingSingularity; }
    VDirection vDirection() const { return m_vDirection; }
    const IntervalNeighborhoods& intervalNeighborhoods() const { return m_intervalNeighborhoods; }

private:
    const Foliation& m_foliation;
    std::size_t m_startingSingularity;
    std::size_t m_depth;
    Mod1NumberIntExWithInfo m_endpoint;
    IntervalNeighborhoods m_intervalNeighborhoods;
    std::vector<std::size_t> m_intervalIntersectionCount;
    VDirection m_vDirection;
    bool m_reachedSaddleConnection;
};

inline SepSegmentIndex index(const SeparatrixSegment& s){
    return { s.hDirection(), s.vDirection(), s.startingSingularity() };
}

}

#endif // SEPARATRIXSEGMENT_H
