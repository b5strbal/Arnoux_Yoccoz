#ifndef SEPSEGMENTCOLLECTIONSGENERAL_H
#define SEPSEGMENTCOLLECTIONSGENERAL_H

#include "Choose.h"
#include "SepSegmentDatabase.h"
//#include "SepSegmentCollection.h"

typedef std::vector<std::list<SeparatrixSegment>::const_iterator> SepSegmentCollection;



class SepSegmentCollections
{
public:
    class iterator{
    public:
        const SepSegmentCollection& operator*() { return m_sepSegmentCollection; }
        iterator& operator++();
        friend bool operator==(const iterator& it1, const iterator& it2);
        friend bool operator!=(const iterator& it1, const iterator& it2);
    private:
        iterator(const SepSegmentCollections& parent);

        SepSegmentCollection m_sepSegmentCollection;
        Choose m_separatrixIndices;
        const SepSegmentCollections& m_parent;
    };

    enum class Mode{
        SEGMENTS_SHIFTED_TO_RIGHT // mode parameter: number of leaf segments
    };


    SepSegmentCollections(SepSegmentDatabase& sepSegmentDatabase,
                            unsigned int maxDepth,
                            Mode mode,
                          unsigned int modeParameter);

    iterator begin() const;
    iterator end() const;

private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    unsigned int m_maxDepth;
    Mode m_mode;
    unsigned int m_modeParameter;
};







//inline bool isAfterLast() const{ return m_segments.empty(); }


//bool isLast(std::list<SeparatrixSegment>::const_iterator it) const;
//void setIndicesToBegin();













#endif // SEPSEGMENTCOLLECTIONSGENERAL_H
