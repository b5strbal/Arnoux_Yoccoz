//
//  WeighedTree.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/3/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "WeighedTree.h"


std::default_random_engine WeighedTree::generator(static_cast<int>(time(NULL)));


WeighedTree::WeighedTree(const std::vector<floating_point_type>& definingList) :
    m_definingList(definingList)
{
    Init(m_definingList);
}


WeighedTree::WeighedTree(const WeighedTree& wt) :
    m_definingList(wt.m_definingList)
{
    Init(m_definingList);
}



WeighedTree::WeighedTree(int NumEdges) :
    m_definingList(randomDefiningList(NumEdges))
{
    Init(m_definingList);
}

WeighedTree::~WeighedTree(){
    delete m_Root;
}




void WeighedTree::Init(const std::vector<floating_point_type>& Weights){
    m_Root = new Node;
    CreateChildren(Weights.begin(), Weights.end(), m_Root);
    SetNumDescendants(m_Root);
}












std::vector<floating_point_type> WeighedTree::randomDefiningList(int numEdges){
    if (numEdges == 1) {    // there is only one tree with one edge
        return std::vector<floating_point_type>(1, 1);
    }
    
    if (numEdges < 3) {
        throw std::runtime_error("A weighed tree must have one or at least three edges.");
    }
    std::vector<floating_point_type> definingList;
    
    std::uniform_real_distribution<floating_point_type> RealDistribution(0,1);
    definingList.push_back(RealDistribution(generator)); // creating the first child of the root

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
            NextSequence = IntDistribution(generator);
        } while (NextSequence == 1 || NextSequence == remains - 1 || (unVisitedLeaves == 1 && NextSequence == 0));
        

        // Now generating the right number of weights.
        for (int i = 0; i < NextSequence; i++) {
            floating_point_type RandomReal = 0;
            while (RandomReal == 0) {                   // making sure the random real generator didn't give us 0
                RandomReal = RealDistribution(generator);
            }
            definingList.push_back(RandomReal);
        }
        definingList.push_back(0);
        remains -= NextSequence;
        unVisitedLeaves += NextSequence - 1;


    }
    return definingList;
}







void WeighedTree::CreateChildren(std::vector<floating_point_type>::const_iterator itBegin,
                                 std::vector<floating_point_type>::const_iterator itEnd,
                                 Node* pNode){
    std::vector<floating_point_type>::const_iterator it = itBegin;
    while (it != itEnd && *it != 0) {
        if (*it < 0) {
            throw std::runtime_error("Weights of weighed trees must be positive.");
        }
        it++;
    }
    if (pNode->m_Parent == NULL && (it - itBegin == 0 || it - itBegin == 2)) {
        throw std::runtime_error("The root of a weighed tree must have 1 or at least 3 neighbors.)");
    }

    if (it != itBegin) {
        pNode->m_NumChildren = static_cast<int>(it - itBegin);
        if (pNode->m_Parent != NULL && pNode->m_NumChildren == 1) {
            throw std::runtime_error("Vertices of degree 2 are not allowed in weighed trees.");
        }
        
        
        pNode->m_Children = new Node[pNode->m_NumChildren];
        for (int i = 0; i < pNode->m_NumChildren; i++) {
            pNode->m_Children[i].m_Parent = pNode;
            pNode->m_Children[i].m_Weight = itBegin[i];
        }
    }
    
    if (++it < itEnd) {
        pNode = NextNode(pNode);
        if (pNode->IsRoot()) {
            throw std::runtime_error("Too many arguments provided for the definition of a weighed tree.");
        } else
            CreateChildren(it, itEnd, pNode);
    }
}



void WeighedTree::SetNumDescendants(Node* pNode){
    int CountDescendants = 0;
    for (int i = 0; i < pNode->m_NumChildren; i++) {
        SetNumDescendants(pNode->m_Children + i);
        CountDescendants += pNode->m_Children[i].m_NumDescendants;
    }
    pNode->m_NumDescendants = CountDescendants + pNode->m_NumChildren;
}



std::vector<int> WeighedTree::getDegrees() const{
    std::vector<int> degrees;
    getDegreesRecursive(degrees, m_Root);
    degrees[0]--;
    std::sort(degrees.begin(), degrees.end(), std::greater<int>());
    return degrees;
}





void WeighedTree::getDegreesRecursive(std::vector<int>& degrees, Node* node) const{
    degrees.push_back(node->m_NumChildren + 1);
    for (int i = 0; i < node->m_NumChildren; i++) {
        getDegreesRecursive(degrees, node->m_Children + i);
    }
}








WeighedTree::Node::~Node(){
    delete [] m_Children;
}






WeighedTree::Node* WeighedTree::NextNode(Node* pNode){
    if (!pNode->IsLastSibling()) {
        return pNode + 1;
    }
    Node* NewNode;
    
    NewNode = pNode->FirstSibling();
    for (int i = 0; i < pNode->GetNumberOfSiblings(); i++) {
        if (NewNode->m_NumChildren > 0) {
            return NewNode->m_Children;
        }
        NewNode++;
    }
    
    return m_Root;
}



