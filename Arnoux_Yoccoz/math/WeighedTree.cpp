//
//  WeighedTree.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/3/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "WeighedTree.h"


std::default_random_engine balazs::WeighedTree::m_randomEngine(static_cast<int>(time(NULL)));


balazs::WeighedTree::WeighedTree(const std::vector<floating_point_type>& definingList) :
    m_definingList(definingList)
{
    m_root = new Node;
    createChildren(definingList.begin(), definingList.end(), m_root);
    m_numEdges = m_root->numDescendants();
}




balazs::WeighedTree balazs::WeighedTree::randomWeighedTree(int numEdges){
    return WeighedTree(randomDefiningList(numEdges));
}









std::vector<balazs::floating_point_type> balazs::WeighedTree::randomDefiningList(int numEdges){
    if (numEdges == 1) {    // there is only one tree with one edge
        return std::vector<floating_point_type>(1, 1);
    }
    
    if (numEdges < 3) {
        throw std::runtime_error("A weighed tree must have one or at least three edges.");
    }
    std::vector<floating_point_type> definingList;
    
    std::uniform_real_distribution<floating_point_type> RealDistribution(0,1);
    definingList.push_back(RealDistribution(m_randomEngine)); // creating the first child of the root

    int remains = numEdges - 1;
    int unVisitedLeaves = 2; // the root also have to be considered is unvisited here
    while (remains > 0) {
        // Now we are generating a random integer between 0 and Remains, excluding 1 and Remains - 1.
        // 1 is excluded, because a sequence of length 1 means there is a single child which results in a degree 2 vertex.
        // Remains - 1 is excluded because after that we would have a single slot left which would result in an only child later.
        // Also, if there is only one unvisited leaf, then it must have children to complete the tree
        std::uniform_int_distribution<int> IntDistribution(0, remains);
        int NextSequence;
        do {
            NextSequence = IntDistribution(m_randomEngine);
        } while (NextSequence == 1 || NextSequence == remains - 1 || (unVisitedLeaves == 1 && NextSequence == 0));
        

        // Now generating the right number of weights.
        for (int i = 0; i < NextSequence; i++) {
            floating_point_type RandomReal = 0;
            while (RandomReal == 0) {                   // making sure the random real generator didn't give us 0
                RandomReal = RealDistribution(m_randomEngine);
            }
            definingList.push_back(RandomReal);
        }
        definingList.push_back(0);
        remains -= NextSequence;
        unVisitedLeaves += NextSequence - 1;


    }
    return definingList;
}







void balazs::WeighedTree::createChildren(std::vector<floating_point_type>::const_iterator itBegin,
                                 std::vector<floating_point_type>::const_iterator itEnd,
                                 Node* pNode){
    std::vector<floating_point_type>::const_iterator it = itBegin;
    while (it != itEnd && *it != 0) {
        if (*it < 0) {
            throw std::runtime_error("Weights of weighed trees must be positive.");
        }
        it++;
    }
    if (pNode->m_parent == NULL && (it - itBegin == 0 || it - itBegin == 2)) {
        throw std::runtime_error("The root of a weighed tree must have 1 or at least 3 neighbors.)");
    }

    if (it != itBegin) {
        pNode->m_numChildren = static_cast<int>(it - itBegin);
        if (pNode->m_parent != NULL && pNode->m_numChildren == 1) {
            throw std::runtime_error("Vertices of degree 2 are not allowed in weighed trees.");
        }
        
        
        pNode->m_children = new Node[pNode->m_numChildren];
        for (std::size_t i = 0; i < pNode->m_numChildren; i++) {
            pNode->m_children[i].m_parent = pNode;
            pNode->m_children[i].m_weight = itBegin[i];
        }
    }
    
    if (++it < itEnd) {
        pNode = nextNode(pNode);
        if (pNode->isRoot()) {
            throw std::runtime_error("Too many arguments provided for the definition of a weighed tree.");
        } else
            createChildren(it, itEnd, pNode);
    }
}



std::vector<int> balazs::WeighedTree::degrees() const{
    std::vector<int> degrees;
    getDegreesRecursive(degrees, m_root);
    degrees[0]--;
    std::sort(degrees.begin(), degrees.end(), std::greater<int>());
    return degrees;
}





void balazs::WeighedTree::getDegreesRecursive(std::vector<int>& degrees, Node* node) const{
    degrees.push_back(node->m_numChildren + 1);
    for (std::size_t i = 0; i < node->m_numChildren; i++) {
        getDegreesRecursive(degrees, node->m_children + i);
    }
}





std::size_t balazs::WeighedTree::Node::numDescendants()
{
    if(m_numChildren == 0)
        return 0;

    std::size_t total = 0;
    for(std::size_t i = 0; i < m_numChildren; i++){
        total += m_children[i].numDescendants();
    }
    return total;
}






balazs::WeighedTree::Node* balazs::WeighedTree::nextNode(Node* pNode){
    if (!pNode->isLastSibling()) {
        return pNode + 1;
    }
    Node* NewNode;
    
    NewNode = pNode->firstSibling();
    for (std::size_t i = 0; i < pNode->numSiblings(); i++) {
        if (NewNode->m_numChildren > 0) {
            return NewNode->m_children;
        }
        NewNode++;
    }
    
    return m_root;
}



