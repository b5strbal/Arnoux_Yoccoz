#ifndef SEPSEGMENTCOLLECTION_H
#define SEPSEGMENTCOLLECTION_H

//#include "Foliation.h"
#include "SepSegmentDatabase.h"
#include <list>


class SepSegmentCollectionBase
{
public:
    SepSegmentCollectionBase(const SepSegmentDatabase&, const std::vector<int>& sepSegmentIndices, int maxdepth);
    inline bool isAfterLast() const{ return m_segments.empty(); }

    unsigned int size() const { return m_segments.size(); }
    virtual void advance() = 0;
    const SeparatrixSegment& operator[](int index) const { return *m_segments[index]; }
    //operator const std::vector<const SeparatrixSegment*>&() { return m_segments; }

protected:
    std::vector<std::list<SeparatrixSegment>::const_iterator> m_segments;
    const SepSegmentDatabase& m_sepSegmentDatabase;
    int m_maxdepth;

    bool isLast(std::list<SeparatrixSegment>::const_iterator it) const;
};




class SepSegmentCollectionDefault : public SepSegmentCollectionBase
{
public:
    SepSegmentCollectionDefault(const SepSegmentDatabase& database, const std::vector<int>& sepSegmentIndices, int maxdepth) :
        SepSegmentCollectionBase(database, sepSegmentIndices, maxdepth)
    {}
    virtual void advance();

};



class SepSegmentCollectionFromRP2 : public SepSegmentCollectionBase
{
public:
    SepSegmentCollectionFromRP2(const SepSegmentDatabaseFromRP2* database, int maxdepth) :
        SepSegmentCollectionBase(*database, {{0, database->m_separatrixPair[0]}}, maxdepth)
    {}
    virtual void advance();

};




#endif // SEPSEGMENTCOLLECTION_H
