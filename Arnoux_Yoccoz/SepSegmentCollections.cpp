#include "SepSegmentCollections.h"

SepSegmentCollections::SepSegmentCollections(SepSegmentDatabase &sepSegmentDatabase,
                                             unsigned int maxDepth,
                                             unsigned int maxInvolvedSingularities,
                                             Mode mode, Direction::LeftOrRight shiftToSide) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_maxInvolvedSingularities(maxInvolvedSingularities),
    m_mode(mode),
    m_shiftToSide(shiftToSide)
{
    assert(maxInvolvedSingularities > 0 && maxInvolvedSingularities <= sepSegmentDatabase.numIntervals());
    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}

SepSegmentCollections::iterator SepSegmentCollections::begin() const
{
    return iterator(*this);
}

SepSegmentCollections::iterator SepSegmentCollections::end() const
{
    return iterator::endIterator(*this);
}





SepSegmentCollections::iterator &SepSegmentCollections::iterator::operator ++()
{
    assert(*this != iterator::endIterator(m_parent));

    switch(m_parent.m_mode)
    {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:

        break;


    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:

        break;
    }

}

bool operator ==(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return it1.m_sepSegmentCollection == it2.m_sepSegmentCollection;
}

bool operator !=(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return it1 == it2 ? false : true;
}

SepSegmentCollections::iterator::iterator(const SepSegmentCollections &parent) :
    m_parent(parent),
    m_sepIndicesChoose(Choose(parent.m_sepSegmentDatabase.numIntervals(), 1))
{
    setInitialSetting();
}

SepSegmentCollections::iterator SepSegmentCollections::iterator::endIterator(const SepSegmentCollections &parent)
{
    return iterator(parent, SepSegmentCollection(), Choose());
}

SepSegmentCollections::iterator::iterator(const SepSegmentCollections &parent,
                                          const SepSegmentCollection &sepSegmentCollection,
                                          const Choose &sepIndicesChoose) :
    m_parent(parent),
    m_sepSegmentCollection(sepSegmentCollection),
    m_sepIndicesChoose(sepIndicesChoose)
{
}



void SepSegmentCollections::iterator::setInitialSetting()
{
    m_sepSegmentCollection.resize(2 * m_sepIndicesChoose.k());

    switch(m_parent.m_mode)
    {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[m_parent.m_shiftToSide][Direction::UP][m_sepIndicesChoose[i]].begin();
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[m_parent.m_shiftToSide][Direction::DOWN][m_sepIndicesChoose[i]].begin();
        }
        break;


    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[Direction::LEFT][Direction::UP][m_sepIndicesChoose[i]].begin();
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[Direction::RIGHT][Direction::UP][m_sepIndicesChoose[i]].begin();
        }
        break;
    }
}











bool SepSegmentCollection::isLast(std::list<SeparatrixSegment>::const_iterator it) const{
    it++;
    return (it == m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[it->direction()][it->startingSingularity()].end() ||
    it->depth() > m_maxdepth);
}




/////////////////////////////////

void SepSegmentCollectionDefault::advance() {
    assert(!isAfterLast());
    int indexToIncrease = static_cast<int>(m_segments.size()) - 1;
    while (indexToIncrease >= 0 && isLast(m_segments[indexToIncrease]) ){
        indexToIncrease--;
    }
    if (indexToIncrease < 0){
        ++m_separatrixIndices;
        if (m_separatrixIndices.isAfterLast()){
            m_segments.clear(); // indicating the element after the last one
        } else {
            setIndicesToBegin();
        }
    } else {
        m_segments[indexToIncrease]++;
        for (unsigned int i = indexToIncrease + 1; i < m_segments.size(); i++){
            m_segments[i] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[m_segments[i]->direction()][m_segments[i]->startingSingularity()].begin();
        }
    }
}



