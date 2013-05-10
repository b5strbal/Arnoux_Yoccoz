#ifndef SEPARATRIXSEGMENT_H
#define SEPARATRIXSEGMENT_H

#include "IntervalNeighborhoods.h"


namespace balazs{


class SeparatrixSegment{
public:
    SeparatrixSegment(const Foliation& foliation, std::size_t startingSingularity, Direction::UpOrDown direction);
    friend std::ostream& operator<<(std::ostream& out, const SeparatrixSegment& s);
    bool reachedSaddleConnection() const { return m_reachedSaddleConnection; }
    void lengthen();
    bool isGood() const;
    bool isCentered() const { return m_endpoint.number().side() == Direction::CENTER; }
    void shiftTo(Direction::LeftOrRight side);

    const Foliation& foliation() const { return m_foliation; }
    Direction::LeftOrRight side() const { return m_endpoint.number().side(); }
    std::size_t depth() const { return m_depth; }
    const Mod1NumberIntExWithInfo& endpoint() const { return m_endpoint; }
    std::size_t startingSingularity() const { return m_startingSingularity; }
    Direction::UpOrDown direction() const { return m_direction; }
    const IntervalNeighborhoods& intervalNeighborhoods() const { return m_intervalNeighborhoods; }

private:
    const Foliation& m_foliation;
    std::size_t m_startingSingularity;
    std::size_t m_depth;
    Mod1NumberIntExWithInfo m_endpoint;
    IntervalNeighborhoods m_intervalNeighborhoods;
    std::vector<std::size_t> m_intervalIntersectionCount;
    Direction::UpOrDown m_direction;
    bool m_reachedSaddleConnection;
};



}

#endif // SEPARATRIXSEGMENT_H
