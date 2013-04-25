#ifndef SEPSEGMENTCOLLECTIONSFROMRP2_H
#define SEPSEGMENTCOLLECTIONSFROMRP2_H

class SepSegmentCollectionsFromRP2
{
public:
    SepSegmentCollectionsFromRP2();
};




class SepSegmentCollectionFromRP2 : public SepSegmentCollection
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
#endif // SEPSEGMENTCOLLECTIONSFROMRP2_H
