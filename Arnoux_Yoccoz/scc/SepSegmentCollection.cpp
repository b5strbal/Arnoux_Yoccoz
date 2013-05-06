#include "SepSegmentCollection.h"

balazs::SepSegmentCollection::SepSegmentCollection(const balazs::SepSegmentDatabase &sepSegmentDatabase) :
    m_sepSegmentDatabase(sepSegmentDatabase)
{
}

balazs::SepSegmentCollection::SepSegmentCollection(const SepSegmentDatabase &sepSegmentDatabase,
                                                    balazs::SepSegmentCollectionMode mode,
                                                    balazs::Direction::LeftOrRight shiftToSide) :
    SepSegmentCollection(sepSegmentDatabase,
                        Choose(howMuchToChooseFrom(mode, sepSegmentDatabase.foliation().numIntervals()), 1),
                        mode,
                        shiftToSide)
{
}

balazs::SepSegmentCollection::SepSegmentCollection(const SepSegmentDatabase &sepSegmentDatabase,
                                                   const Choose &sepIndicesChoose,
                                                   SepSegmentCollectionMode mode,
                                                   Direction::LeftOrRight shiftToSide) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_sepIndicesChoose(sepIndicesChoose),
    m_mode(mode),
    m_shiftToSide(shiftToSide)
{
    if(mode == SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2){
        assert(dynamic_cast<const FoliationFromRP2*>(&foliation()) != nullptr);
        m_choiceOfSingularities_RP2.reserve(foliationFromRP2().numIntervals() / 2);
        for(std::size_t i = 0; i < foliationFromRP2().numIntervals(); i++){
            if(foliationFromRP2().intervalPermutationBeforeHalfTwist()[i] > i){
                m_choiceOfSingularities_RP2.push_back(i);
            }
        }
    }
}


balazs::sepSegmentDatabaseConstIterator_t balazs::SepSegmentCollection::operator [](std::size_t index) const
{
    return m_segments[index];
}






void balazs::SepSegmentCollection::setInitialSetting(const Choose &sepIndicesChoose)
{
    m_sepIndicesChoose = sepIndicesChoose;

    switch(m_mode)
    {
    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
        m_segments.resize(2 * m_sepIndicesChoose.k());

        for(std::size_t i = 0; i < m_sepIndicesChoose.k(); i++){
            m_segments[2 * i] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::UP, m_sepIndicesChoose[i]);
            m_segments[2 * i + 1] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::DOWN, m_sepIndicesChoose[i]);
        }

        break;


    case SepSegmentCollectionMode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        m_segments.resize(2 * m_sepIndicesChoose.k());

        for(std::size_t i = 0; i < m_sepIndicesChoose.k(); i++){
            m_segments[2 * i] = m_sepSegmentDatabase.firstGoodSegment(Direction::LEFT, Direction::UP, m_sepIndicesChoose[i]);
            m_segments[2 * i + 1] = m_sepSegmentDatabase.firstGoodSegment(Direction::RIGHT, Direction::UP, m_sepIndicesChoose[i]);
        }

        break;


    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        m_segments.resize(4 * m_sepIndicesChoose.k());


        for(std::size_t i = 0; i < m_sepIndicesChoose.k(); i++){
            std::size_t firstIndex = m_choiceOfSingularities_RP2[m_sepIndicesChoose[i]];
            std::size_t secondIndex = foliationFromRP2().intervalPermutationBeforeHalfTwist()[firstIndex];

            m_segments[4 * i] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::UP, firstIndex);
            m_segments[4 * i + 1] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::DOWN, firstIndex);

            m_segments[4 * i + 2] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::UP, secondIndex);
            m_segments[4 * i + 3] = m_sepSegmentDatabase.firstGoodSegment(m_shiftToSide, Direction::DOWN, secondIndex);
        }


        break;
    }
}






void balazs::SepSegmentCollection::advance(std::size_t maxDepth,
                                           std::size_t maxInvolvedSingularities)
{
    assert(!isEmpty());

    int indexToIncrease = m_segments.size() - 1;
    while (indexToIncrease >= 0 && m_sepSegmentDatabase.isLast(
               m_segments[indexToIncrease], maxDepth) ){
        indexToIncrease--;
    }

    if (indexToIncrease >= 0){
        // One of the sepSegments is not the last one, so we can lengthen that, and set the
        // ones after it to the first.

        std::size_t setToFirstAfterThis;

        switch(m_mode)
        {
        case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
            m_segments[indexToIncrease]++;
            setToFirstAfterThis = indexToIncrease;

            break;

        case SepSegmentCollectionMode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
            assert(indexToIncrease % 2 == 1);
            m_segments[indexToIncrease]++;
            m_segments[indexToIncrease - 1]++;
            assert(m_segments[indexToIncrease]->depth() ==
                   m_segments[indexToIncrease - 1]->depth());
            setToFirstAfterThis = indexToIncrease;

            break;

        case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
            assert(indexToIncrease % 4 >= 2);
            m_segments[indexToIncrease]++;
            std::size_t otherToIncrease;
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
            m_segments[otherToIncrease]++;

        }


        for (std::size_t i = setToFirstAfterThis + 1; i < m_segments.size(); i++){
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

            if(this->numInvolvedSingularities() < maxInvolvedSingularities){
                // We can make subsets larger.

                setInitialSetting(Choose(howMuchToChooseFrom(m_mode, foliation().numIntervals()),
                                         m_sepIndicesChoose.k() + 1));

            } else {
                // We reached the very last collection, there is no next one.

                clear();
            }
        }
    }
}










void balazs::SepSegmentCollection::setSegmentToFirst(std::size_t segmentIndex)
{
    m_segments[segmentIndex] =
            m_sepSegmentDatabase.firstGoodSegment(m_segments[segmentIndex]);
}





std::size_t balazs::SepSegmentCollection::numInvolvedSingularities() const
{
    switch (m_mode) {
    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
    case SepSegmentCollectionMode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        return m_sepIndicesChoose.k();
    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        return 2 * m_sepIndicesChoose.k();
    }
}




const balazs::Foliation &balazs::SepSegmentCollection::foliation() const
{
    return m_sepSegmentDatabase.foliation();
}



const balazs::FoliationFromRP2 &balazs::SepSegmentCollection::foliationFromRP2() const{
    assert(m_mode == SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2);
    return dynamic_cast<const FoliationFromRP2&>(m_sepSegmentDatabase.foliation());
}




std::size_t balazs::howMuchToChooseFrom(SepSegmentCollectionMode mode,
                                                               std::size_t numSingularities)
{
    switch (mode) {
    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE:
    case SepSegmentCollectionMode::WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY:
        return numSingularities;
    case SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2:
        return numSingularities / 2;
    }
}

