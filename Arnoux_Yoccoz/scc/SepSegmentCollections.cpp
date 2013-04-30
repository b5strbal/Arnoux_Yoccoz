#include "SepSegmentCollections.h"

balazs::SepSegmentCollections::SepSegmentCollections(SepSegmentDatabase &sepSegmentDatabase,
                                             unsigned int maxDepth,
                                             unsigned int maxInvolvedSingularities,
                                             Mode mode, Direction::LeftOrRight shiftToSide) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_maxInvolvedSingularities(maxInvolvedSingularities),
    m_mode(mode),
    m_shiftToSide(shiftToSide)
{
    assert(maxInvolvedSingularities > 0 && maxInvolvedSingularities <= sepSegmentDatabase.foliation().numIntervals());
    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}

balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::begin() const
{
    return iterator(*this);
}

balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::end() const
{
    return iterator::endIterator(*this);
}





balazs::SepSegmentCollections::iterator &balazs::SepSegmentCollections::iterator::operator ++()
{
    assert(*this != iterator::endIterator(m_parent));


    int indexToIncrease = m_sepIndicesChoose.k() - 1;
    while (indexToIncrease >= 0 && m_parent.m_sepSegmentDatabase.isLast(
               m_sepSegmentCollection[indexToIncrease], m_parent.m_maxDepth) ){
        indexToIncrease--;
    }

    if (indexToIncrease >= 0){
        // One of the sepSegments is not the last one, so we can lengthen that, and set the
        // ones after it to the first.

        switch(m_parent.m_mode)
        {
        case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
            m_sepSegmentCollection[indexToIncrease]++;

            break;



        case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
            assert(indexToIncrease % 2 == 1);
            m_sepSegmentCollection[indexToIncrease]++;
            m_sepSegmentCollection[indexToIncrease - 1]++;
            assert(m_sepSegmentCollection[indexToIncrease]->depth() ==
                   m_sepSegmentCollection[indexToIncrease - 1]->depth());
            break;
        }

        for (unsigned int i = indexToIncrease + 1; i < m_sepSegmentCollection.size(); i++){
            auto it = m_sepSegmentCollection[i];
            m_parent.m_sepSegmentDatabase.firstGoodSegment(it->side(),
                                                           it->direction(), it->startingSingularity());
        }



    } else {
        // All of the segments are the last one, so we have to iterate m_sepIndicesChoose,,
        // the choice of separatrix choices.

        ++m_sepIndicesChoose;

        if(!m_sepIndicesChoose.isAfterLast()){
            // The new Choose is still valid, therefore, we can set all sepSegments to the first one.

            setInitialSetting(m_sepIndicesChoose);

        } else {
            // Even m_sepIndicesChoose was the last one. Now we can try to increase the k-value of Choose,
            // i.e. take larger subsets.

            if(m_sepIndicesChoose.k() < m_parent.m_maxInvolvedSingularities){
                // We can make subsets larger.

                setInitialSetting(Choose(m_sepIndicesChoose.n(), m_sepIndicesChoose.k() + 1));

            } else {
                // We reached the very last collection, there is no next one.

                setToEndIterator();
            }


        }


    }











    return *this;
}

bool balazs::operator ==(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return *it1 == *it2;
}

bool balazs::operator !=(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return it1 == it2 ? false : true;
}

balazs::SepSegmentCollections::iterator::iterator(const SepSegmentCollections &parent) :
    m_parent(parent)
{
    setInitialSetting(Choose(parent.m_sepSegmentDatabase.foliation().numIntervals(), 1));
}

balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::iterator::endIterator(const SepSegmentCollections &parent)
{
    balazs::SepSegmentCollections::iterator it(parent);
    it.setToEndIterator();
    return it;
}

balazs::SepSegmentCollections::iterator::iterator(const SepSegmentCollections &parent,
                                          const SepSegmentCollection &sepSegmentCollection,
                                          const Choose &sepIndicesChoose) :
    m_parent(parent),
    m_sepSegmentCollection(sepSegmentCollection),
    m_sepIndicesChoose(sepIndicesChoose)
{
}

void balazs::SepSegmentCollections::iterator::setToEndIterator()
{
    m_sepSegmentCollection = {};
    m_sepIndicesChoose = Choose();
}



void balazs::SepSegmentCollections::iterator::setInitialSetting(const Choose &sepIndicesChoose)
{
    m_sepIndicesChoose = sepIndicesChoose;
    m_sepSegmentCollection.resize(2 * m_sepIndicesChoose.k());

    switch(m_parent.m_mode)
    {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::UP, m_sepIndicesChoose[i]);
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::DOWN, m_sepIndicesChoose[i]);


        }
        break;


    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.firstGoodSegment(Direction::LEFT, Direction::UP, m_sepIndicesChoose[i]);
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.firstGoodSegment(Direction::RIGHT, Direction::UP, m_sepIndicesChoose[i]);
        }
        break;
    }
}









