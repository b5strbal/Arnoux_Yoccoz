#ifndef CHOOSE_H
#define CHOOSE_H

#include <vector>

/*!
 * \brief   The Choose class represents a subset of size k of a n-element set.
 * \author  Balazs Strenner, strenner@math.wisc.edu
 * \date    April 18, 2013
 *
 * The n-element set is represented as the integers from 0 to n-1. The k-element subset is
 * therefore a set of k distinct numbers between 0 and n-1.
 * Internally, these k elements are ordered in a container, and one can access them using the
 * subscript operator. The begin() and end() methods allow the use of the range-based for loop,
 * so for instance for(i : choose) lets i iterate over the elements of the k-set in the Choose
 * class instance choose.
 */

class Choose{
public:

    //! Constructs the subset of the first k elements.
    Choose(int n, int k);

    //! Steps to the next subset in the lexicographical order of subsets
    Choose& operator++();

    //! Returns n, the size of the base set.
    int n() const { return m_n; }

    //! Returns k, the size of the subsets.
    int k() const { return m_k; }

    //! Returns true if the current subset is past the last subset, i.e. not valid anymore.
    inline bool isAfterLast() const { return m_chosenIndices.empty(); }

    //! Returns the i'th element of the subset.
    int operator[](int i) { return m_chosenIndices[i]; }

    //! Returns an iterator pointing to the beginning of the vector storing the elements of the subset.
    std::vector<int>::const_iterator begin() const { return m_chosenIndices.begin(); }

    //! Returns a pointer pointing after the last ele
    std::vector<int>::const_iterator end() const { return m_chosenIndices.end(); }


    // operator const std::vector<int>&(){ return m_chosenIndices; }

private:
    int m_n;    // The size of the base set.
    int m_k;    // The size of the subset.
    std::vector<int> m_chosenIndices;   // The list of elements in the subset.
};

#endif // CHOOSE_H
