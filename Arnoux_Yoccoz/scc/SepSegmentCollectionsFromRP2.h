#ifndef SEPSEGMENTCOLLECTIONSFROMRP2_H
#define SEPSEGMENTCOLLECTIONSFROMRP2_H

#include "SepSegmentCollections.h"

namespace balazs{




class SepSegmentCollectionsFromRP2 : public SepSegmentCollections
{
public:
    SepSegmentCollectionsFromRP2(SepSegmentDatabaseFromRP2 &database, unsigned int maxDepth, Mode mode,
                                 unsigned int modeParameter);

    virtual void advance();
private:
    // Choose m_separatrixIndices is inherited, but now we don't use is to store all the indices, just half of them.
    // The other half is then determined by taking the pairs of these.
    std::vector<int> m_allowedSeparatrixIndices;

    void setIndicesToBegin(){}
};



}

#endif // SEPSEGMENTCOLLECTIONSFROMRP2_H
