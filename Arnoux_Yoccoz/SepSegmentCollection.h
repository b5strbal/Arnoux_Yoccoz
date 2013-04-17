#ifndef SEPSEGMENTCOLLECTION_H
#define SEPSEGMENTCOLLECTION_H

//#include "Foliation.h"
#include "SepSegmentDatabase.h"
#include <list>
#include "Choose.h"


class SepSegmentCollectionBase
{
public:
    SepSegmentCollectionBase(const SepSegmentDatabase&, int numLeafSegments, int maxdepth);
    inline bool isAfterLast() const{ return m_segments.empty(); }
    unsigned int size() const { return m_segments.size(); }
    virtual void advance() = 0;
    const SeparatrixSegment& operator[](int index) const { return *m_segments[index]; }
    //operator const std::vector<const SeparatrixSegment*>&() { return m_segments; }

protected:
    std::vector<std::list<SeparatrixSegment>::const_iterator> m_segments;
    const SepSegmentDatabase& m_sepSegmentDatabase;
    int m_maxdepth;
    Choose m_separatrixIndices;

    bool isLast(std::list<SeparatrixSegment>::const_iterator it) const;
    void setIndicesToBegin();
};




class SepSegmentCollectionDefault : public SepSegmentCollectionBase
{
public:
    SepSegmentCollectionDefault(const SepSegmentDatabase& database, int numLeafSegments, int maxdepth) :
        SepSegmentCollectionBase(database, numLeafSegments, maxdepth)
    {}
    virtual void advance();

};



class SepSegmentCollectionFromRP2 : public SepSegmentCollectionBase
{
public:
    SepSegmentCollectionFromRP2(const SepSegmentDatabaseFromRP2& database, int numLeafSegments, int maxdepth);

    virtual void advance();
private:
    // Choose m_separatrixIndices is inherited, but now we don't use is to store all the indices, just half of them.
    // The other half is then determined by taking the pairs of these.
    std::vector<int> m_allowedSeparatrixIndices;

    void setIndicesToBegin();
};




#endif // SEPSEGMENTCOLLECTION_H
