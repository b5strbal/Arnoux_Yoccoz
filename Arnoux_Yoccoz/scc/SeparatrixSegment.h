#ifndef SEPARATRIXSEGMENT_H
#define SEPARATRIXSEGMENT_H

#include "IntervalNeighborhoods.h"


namespace balazs{


class SeparatrixSegment{
public:
    SeparatrixSegment(const Foliation& foliation, int startingSingularity, Direction::UpOrDown direction);
    friend std::ostream& operator<<(std::ostream& out, const SeparatrixSegment& s);
    bool reachedSaddleConnection() const { return m_reachedSaddleConnection; }
    void lengthen();
    bool isGood() const;
    bool isCentered() const { return m_endpoint.number().side() == Direction::CENTER; }
    void shiftTo(Direction::LeftOrRight side);

    const Foliation& foliation() const { return m_foliation; }
    Direction::LeftOrRight side() const { return m_endpoint.number().side(); }
    int depth() const { return m_depth; }
    const Mod1NumberIntExWithInfo& endpoint() const { return m_endpoint; }
    int startingSingularity() const { return m_startingSingularity; }
    Direction::UpOrDown direction() const { return m_direction; }
    const IntervalNeighborhoods& intervalNeighborhoods() const { return m_intervalNeighborhoods; }

private:
    const Foliation& m_foliation;
    int m_startingSingularity;
    int m_depth;
    Mod1NumberIntExWithInfo m_endpoint;
    IntervalNeighborhoods m_intervalNeighborhoods;
    std::vector<int> m_intervalIntersectionCount;
    Direction::UpOrDown m_direction;
    bool m_reachedSaddleConnection;
};



}

#endif // SEPARATRIXSEGMENT_H
