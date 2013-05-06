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

namespace balazs{

class Choose{
public:
    Choose() : Choose(1, 1) {} // default constructor
    Choose(std::size_t n, std::size_t k); // {0, 1, ..., k-1} in base set {0, 1, ..., n-1}
    std::size_t n() const { return m_n; }
    std::size_t k() const { return m_k; }
    Choose& operator++();
    bool isAfterLast() const; // Returns true if the current subset is past the last subset, i.e. not valid anymore.
    std::size_t operator[](std::size_t i);  // Returns the i'th element of the subset.
    std::vector<std::size_t>::const_iterator begin() const { return m_chosenIndices.begin(); }
    std::vector<std::size_t>::const_iterator end() const { return m_chosenIndices.end(); }


    // operator const std::vector<int>&(){ return m_chosenIndices; }

private:
    std::size_t m_n;    // The size of the base set.
    std::size_t m_k;    // The size of the subset.
    std::vector<std::size_t> m_chosenIndices;   // The list of elements in the subset.
};


}

#endif // CHOOSE_H
