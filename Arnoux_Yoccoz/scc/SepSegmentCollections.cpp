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
    assert(maxInvolvedSingularities > 0 &&
           maxInvolvedSingularities <= foliation().numIntervals());
    if(mode == Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2){
        assert(dynamic_cast<const FoliationFromRP2*>(&foliation()) != nullptr);
        assert(maxInvolvedSingularities % 2 == 0);
        m_choiceOfSingularities_RP2.reserve(foliationFromRP2().numIntervals() / 2);
        for(unsigned int i = 0; i < foliationFromRP2().numIntervals(); i++){
            if(foliationFromRP2().intervalPermutationBeforeHalfTwist()[i] > i){
                m_choiceOfSingularities_RP2.push_back(i);
            }
        }
    }


    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}




balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::begin() const
{
    return iterator::beginIterator(*this);
}





balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::end() const
{
    return iterator::endIterator(*this);
}



const balazs::Foliation &balazs::SepSegmentCollections::foliation() const
{
    return m_sepSegmentDatabase.foliation();
}

const balazs::FoliationFromRP2 &balazs::SepSegmentCollections::foliationFromRP2() const{
    assert(m_mode == Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2);
    return dynamic_cast<const FoliationFromRP2&>(m_sepSegmentDatabase.foliation());
}

























balazs::SepSegmentCollections::iterator::iterator(const balazs::SepSegmentCollections &parent) :
    m_parent(parent)
{
    setInitialSetting(Choose(howMuchToChooseFrom(), 1));
}





balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::iterator::beginIterator(const SepSegmentCollections& parent)
{
    return iterator(parent);
}




balazs::SepSegmentCollections::iterator balazs::SepSegmentCollections::iterator::endIterator(const SepSegmentCollections &parent)
{
    balazs::SepSegmentCollections::iterator it(parent);
    it.setToEndIterator();
    return it;
}




void balazs::SepSegmentCollections::iterator::setToEndIterator()
{
    m_sepSegmentCollection = {};
    m_sepIndicesChoose = Choose();
}





void balazs::SepSegmentCollections::iterator::setInitialSetting(const Choose &sepIndicesChoose)
{
    m_sepIndicesChoose = sepIndicesChoose;

    switch(m_parent.m_mode)
    {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
        m_sepSegmentCollection.resize(2 * m_sepIndicesChoose.k());

        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::UP, m_sepIndicesChoose[i]);
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::DOWN, m_sepIndicesChoose[i]);
        }

        break;


    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        m_sepSegmentCollection.resize(2 * m_sepIndicesChoose.k());

        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            m_sepSegmentCollection[2 * i] = m_parent.m_sepSegmentDatabase.firstGoodSegment(Direction::LEFT, Direction::UP, m_sepIndicesChoose[i]);
            m_sepSegmentCollection[2 * i + 1] = m_parent.m_sepSegmentDatabase.firstGoodSegment(Direction::RIGHT, Direction::UP, m_sepIndicesChoose[i]);
        }

        break;


    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        m_sepSegmentCollection.resize(4 * m_sepIndicesChoose.k());


        for(unsigned int i = 0; i < m_sepIndicesChoose.k(); i++){
            unsigned int firstIndex = m_parent.m_choiceOfSingularities_RP2[m_sepIndicesChoose[i]];
            unsigned int secondIndex = m_parent.foliationFromRP2().intervalPermutationBeforeHalfTwist()[firstIndex];

            m_sepSegmentCollection[4 * i] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::UP, firstIndex);
            m_sepSegmentCollection[4 * i + 1] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::DOWN, firstIndex);

            m_sepSegmentCollection[4 * i + 2] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::UP, secondIndex);
            m_sepSegmentCollection[4 * i + 3] = m_parent.m_sepSegmentDatabase.firstGoodSegment(m_parent.m_shiftToSide, Direction::DOWN, secondIndex);
        }


        break;
    }
}



unsigned int balazs::SepSegmentCollections::iterator::numInvolvedSingularities() const
{
    switch (m_parent.m_mode) {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        return m_sepIndicesChoose.k();
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        return 2 * m_sepIndicesChoose.k();
    }
}



unsigned int balazs::SepSegmentCollections::iterator::howMuchToChooseFrom() const
{
    switch (m_parent.m_mode) {
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
    case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        return m_parent.foliation().numIntervals();
    case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        return m_parent.foliation().numIntervals() / 2;
    }
}










balazs::SepSegmentCollections::iterator &balazs::SepSegmentCollections::iterator::operator ++()
{
    assert(*this != iterator::endIterator(m_parent));

    int indexToIncrease = m_sepSegmentCollection.size() - 1;
    while (indexToIncrease >= 0 && m_parent.m_sepSegmentDatabase.isLast(
               m_sepSegmentCollection[indexToIncrease], m_parent.m_maxDepth) ){
        indexToIncrease--;
    }

    if (indexToIncrease >= 0){
        // One of the sepSegments is not the last one, so we can lengthen that, and set the
        // ones after it to the first.

        unsigned int setToFirstAfterThis;

        switch(m_parent.m_mode)
        {
        case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
            m_sepSegmentCollection[indexToIncrease]++;
            setToFirstAfterThis = indexToIncrease;

            break;

        case Mode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
            assert(indexToIncrease % 2 == 1);
            m_sepSegmentCollection[indexToIncrease]++;
            m_sepSegmentCollection[indexToIncrease - 1]++;
            assert(m_sepSegmentCollection[indexToIncrease]->depth() ==
                   m_sepSegmentCollection[indexToIncrease - 1]->depth());
            setToFirstAfterThis = indexToIncrease;

            break;

        case Mode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
            assert(indexToIncrease % 4 >= 2);
            m_sepSegmentCollection[indexToIncrease]++;
            unsigned int otherToIncrease;
            switch (indexToIncrease % 4) {
//            case 0:
//                otherToIncrease = indexToIncrease + 3;
//                break;
//            case 1:
//                otherToIncrease = indexToIncrease + 1;
//                break;
            case 2:
                otherToIncrease = indexToIncrease - 1;
                setToFirstAfterThis = indexToIncrease + 1;
                setSegmentToFirst(indexToIncrease - 2);
                setSegmentToFirst(indexToIncrease + 1);
                break;
            case 3:
                otherToIncrease = indexToIncrease - 3;
                setToFirstAfterThis = indexToIncrease;

                break;
            }
            m_sepSegmentCollection[otherToIncrease]++;

        }


        for (unsigned int i = setToFirstAfterThis + 1; i < m_sepSegmentCollection.size(); i++){
             setSegmentToFirst(i);
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

            if(numInvolvedSingularities() < m_parent.m_maxInvolvedSingularities){
                // We can make subsets larger.

                setInitialSetting(Choose(howMuchToChooseFrom(), m_sepIndicesChoose.k() + 1));

            } else {
                // We reached the very last collection, there is no next one.

                setToEndIterator();
            }
        }
    }
    return *this;
}




void balazs::SepSegmentCollections::iterator::setSegmentToFirst(unsigned int segmentIndex)
{
    m_sepSegmentCollection[segmentIndex] =
            m_parent.m_sepSegmentDatabase.firstGoodSegment(m_sepSegmentCollection[segmentIndex]);
}












bool balazs::operator ==(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return *it1 == *it2;
}

bool balazs::operator !=(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return it1 == it2 ? false : true;
}








