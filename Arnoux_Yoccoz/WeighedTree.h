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
#include "global.h"
#include <cassert>

/**
 * @brief   A WeighedTree object represents a tree embedded into the plane with positive weights on its edges.
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 * @details The motivation for this class is that measured foliations on the disk can be conveniently represented by WeighedTrees. 
 *          See FoliationDisk for more details.
 *
 *          There are some restrictions on the weighed trees considered.
 *
 *          1) The degree of every vertex is either 1 or greater than 3. Vertices of degree 2 are not allowed.
 *
 *          2) The tree has to have at least 4 vertices.
 *
 * @see     FoliationDisk
 */

class WeighedTree{
    friend class InitArguments_IntervalExchangeFoliationDisk;
private:
    /**
     * @brief
     */
    struct Node{
        
        floating_point_type m_Weight = 0;
        Node* m_Parent = NULL;
        int m_NumChildren = 0;
        Node* m_Children = NULL;
        int m_NumDescendants = 0;
        
        ~Node();
        
        inline Node* FirstChild() { return m_NumChildren == 0 ? NULL : m_Children; }
        inline Node* LastChild() { return m_NumChildren == 0 ? NULL : m_Children + (m_NumChildren - 1); }
        inline Node* FirstSibling() { return IsRoot() ? this : m_Parent->FirstChild(); };
        //Node* Root();
        
        inline bool IsRoot() { return m_Parent == NULL ? true : false; }
        bool IsLastSibling() { return IsRoot() || m_Parent->LastChild() == this ? true : false; }
        int GetNumberOfSiblings(){ return IsRoot() ? 1 : m_Parent->m_NumChildren; }
    };
    
public:
    /**
     * @brief   Constructor taking the list of weights separated by zeroes as arguments.
     * @details The input is a vector of non-negative numbers. To get a coding of a WeighedTree by such a sequence, 
     *          first pick a root for the tree, and draw it "hanging down" from the root.
     *          List the weights of the edges hanging down from root from left to right include a 0 at the end as the separator. 
     *          Proceed to the next generation, enter the weights hanging from the first vertex in the generation and incldue 0, 
     *          and so on until all weights are entered. The ending zeroes at the end of the whole sequence can be omitted.
     *
     *          Example: the sequence 0.1 0.2 0.3 0 0.1 0.2 0.3 0 0.5 0.6 0 0.9 0.5 encodes a weighed tree with 11 vertices, 
     *          7 of them are leaves, 3 vertices are of degree 3, one vertex is of degree 4.
     */
    WeighedTree(std::vector<floating_point_type> Weights);
    
    /**
     * @brief   Constructor for a random tree with prescribed number of edges, with weights being in the interval (0,1].
     */
    WeighedTree(int NumEdges);
    
    /**
     * @brief   Destructor.
     */
    ~WeighedTree();
    
    WeighedTree(const WeighedTree&);

    int getNumEdges() const { return m_Root->m_NumDescendants; }
        
    void getDegrees(std::vector<int>& degrees) const;

    
private:
    Node* m_Root;
    std::vector<floating_point_type> m_definingList;
    
    void fillInLengthsAndPairing(std::vector<floating_point_type>& lengths, std::vector<int>& pairing, int StartingIndex, Node* pNode) const;

    
    /**
     * @brief   Initializes the object for the list of weights separated by zeroes as the argument. Called by the constructors.
     */
    void Init(std::vector<floating_point_type> Weights);
    
    /**
     * @brief   Generates a random list of weights that is used to construct a random object. 
     *          It is guaranteed that a valid sequence is produced, no further assertions are needed.
     */
    void GenerateRandomWeights(std::vector<floating_point_type>& Weights, int NumEdges);
    Node* NextNode(Node*);
    void CreateChildren(std::vector<floating_point_type>::iterator itBegin, std::vector<floating_point_type>::iterator itEnd, Node* pNode);
    void SetNumDescendants(Node* pNode);
    
    void getDegreesRecursive(std::vector<int>& degrees, Node* node) const;
    
    WeighedTree& operator=(const WeighedTree&);
};













#endif /* defined(__Arnoux_Yoccoz__WeighedTree__) */
