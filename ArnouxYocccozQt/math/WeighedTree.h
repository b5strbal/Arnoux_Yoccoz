/**
 * @file WeighedTree.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the WeighedTree class.
 */

#ifndef __Arnoux_Yoccoz__WeighedTree__
#define __Arnoux_Yoccoz__WeighedTree__

#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>
#include "../global.h"
#include <cassert>

namespace balazs{


/*!
 * \brief   A WeighedTree object represents a tree embedded into the plane with positive weights on its edges.
 * \author  Balazs Strenner, strenner@math.wisc.edu
 * \date    April 27, 2013
 *
 * The motivation for this class is that measured foliations on the disk can be conveniently represented
 * by WeighedTrees. See FoliationDisk for more details.
 *
 * There are some restrictions on the weighed trees considered:
 * 1) The degree of every vertex is either 1 or greater than 3. Vertices of degree 2 are not allowed.
 * 2) The tree has to have at least 1 edge.
 *
 * We can (non-uniquely) encode a WeighedTree with a list of weights, separated by zeroes.
 * For this, pick a root for the tree, and draw it "hanging down" from the root.
 * List the weights of the edges hanging down from root from left to right and include a 0 at the end as a separator.
 * Proceed to the next generation, enter the weights hanging from the first vertex in the generation and incldue 0,
 * and so on until all weights are entered. The ending zeroes at the end of the whole sequence can be omitted.
 *
 * Example: the sequence 0.1 0.2 0.3 0 0.1 0.2 0.3 0 0.5 0.6 0 0.9 0.5 encodes a weighed tree with 11 vertices,
 * 7 of them are leaves, 3 vertices are of degree 3, one vertex is of degree 4. (The root has 3 children with
 * weights 0.1, 0.2, 0.3. The first child has again three children with the same weights on the edges.
 * The second and third child has two children. I.e. one root, 3 vertices in the first generation,
 * 7 in the second generatrion).
 *
 *
 *
 *
 * \see     FoliationDisk
 */

class WeighedTree{
    friend class IntervalPairing;
public:
    WeighedTree(const std::vector<floating_point_type>& definingList);
    WeighedTree(const WeighedTree& wt) : WeighedTree(wt.m_definingList) {}
    static WeighedTree randomWeighedTree(int numEdges); // Random WeighedTree with prescribed number of edges
    ~WeighedTree() { delete m_root; }
    
    std::size_t numEdges() const { return m_numEdges; }
    std::vector<int> degrees() const;


private:
    struct Node{
        floating_point_type m_weight = 0;
        Node* m_parent = NULL;
        std::size_t m_numChildren = 0;
        Node* m_children = NULL;

        ~Node(){ delete [] m_children; }

        inline Node* firstChild() { return m_numChildren == 0 ? NULL : m_children; }
        inline Node* lastChild() { return m_numChildren == 0 ? NULL : m_children + (m_numChildren - 1); }
        inline Node* firstSibling() { return isRoot() ? this : m_parent->firstChild(); }

        inline bool isRoot() const { return m_parent == NULL ? true : false; }
        bool isLastSibling() const { return isRoot() || m_parent->lastChild() == this ? true : false; }
        std::size_t numSiblings() const { return isRoot() ? 1 : m_parent->m_numChildren; }
        std::size_t numDescendants();
    };



    // Generates a random list of weights that is used to construct a random object.
    // It is guaranteed that a valid sequence is produced, no further assertions are needed.
    static std::vector<floating_point_type> randomDefiningList(int NumEdges);
    Node* nextNode(Node*);
    void createChildren(std::vector<floating_point_type>::const_iterator itBegin,
                        std::vector<floating_point_type>::const_iterator itEnd,
                        Node* pNode);
    
    void getDegreesRecursive(std::vector<int>& degrees, Node* node) const;


    WeighedTree& operator=(const WeighedTree&); // assignment operator hidden



    std::vector<floating_point_type> m_definingList;
    Node* m_root;
    std::size_t m_numEdges;
    static std::default_random_engine m_randomEngine;
};



}










#endif /* defined(__Arnoux_Yoccoz__WeighedTree__) */
