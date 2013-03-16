//
//  WeighedTree.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 2/3/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//

#include "WeighedTree.h"



WeighedTree::WeighedTree(std::vector<floating_point_type> Weights){
    Init(Weights);
}



WeighedTree::WeighedTree(int NumEdges){
    std::vector<floating_point_type> Weights;
    GenerateRandomWeights(Weights, NumEdges);
    Init(Weights);
}

WeighedTree::~WeighedTree(){
    delete m_Root;
}




void WeighedTree::Init(std::vector<floating_point_type> Weights){
    m_Root = new Node;
    CreateChildren(Weights.begin(), Weights.end(), m_Root);
    SetNumDescendants(m_Root);
}





void WeighedTree::generateLengthsAndPairing(std::vector<floating_point_type>& lengths, std::vector<int>& pairing) const{
    lengths.resize(2 * getNumEdges());
    pairing.resize(2 * getNumEdges());
    fillInLengthsAndPairing(lengths, pairing, 0, m_Root);
}



void WeighedTree::fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                          std::vector<int>& pairing, int StartingIndex, Node* pNode) const{
    int ChildrenStartingIndex = StartingIndex;
    for (int i = 0; i < pNode->m_NumChildren; i++) {
        int pair = ChildrenStartingIndex + 2 * pNode->m_Children[i].m_NumDescendants + 1;
        lengths[ChildrenStartingIndex] = lengths[pair] = pNode->m_Children[i].m_Weight;
        pairing[ChildrenStartingIndex] = pair;
        pairing[pair] = ChildrenStartingIndex;
        fillInLengthsAndPairing(lengths, pairing, ChildrenStartingIndex + 1, pNode->m_Children + i);
        ChildrenStartingIndex = pair + 1;
    }
}






void WeighedTree::GenerateRandomWeights(std::vector<floating_point_type>& Weights, int NumEdges){
    assert(NumEdges >= 3);
    std::default_random_engine generator(static_cast<int>(time(NULL)));
    std::uniform_real_distribution<floating_point_type> RealDistribution(0,1);
    Weights.clear();
    Weights.push_back(RealDistribution(generator));

    int Remains = NumEdges - 1;
    while (Remains > 0) {
        // Now we are generating a random integer between 0 and Remains, excluding 1 and Remains - 1.
        // 1 is excluded, because a sequence of length 1 means there is a single child which results in a degree 2 vertex.
        // Remains - 1 is excluded because after that we would have a single slot left which would result in an only child later.
        // So we generates a ranodm integer between 1 and Remains - 1 and then if we replace 1 by 0, and Remains-1 by Remains.
        std::uniform_int_distribution<int> IntDistribution(1, Remains - 1);
        int NextSequence = IntDistribution(generator);
        if (NextSequence == 1) {
            NextSequence = 0;
        } else
            if (NextSequence == Remains - 1) {
                NextSequence = Remains;
            }
        
        // Now generating the right number of weights.
        for (int i = 0; i < NextSequence; i++) {
            floating_point_type RandomReal = 0;
            while (RandomReal == 0) {                   // making sure the random real generator didn't give us 0
                RandomReal = RealDistribution(generator);
            }
            Weights.push_back(RandomReal);
        }
        Weights.push_back(0);
        Remains -= NextSequence;
    }
}





void WeighedTree::CreateChildren(std::vector<floating_point_type>::iterator itBegin, std::vector<floating_point_type>::iterator itEnd, Node* pNode){
    std::vector<floating_point_type>::iterator it = itBegin;
    while (it != itEnd && *it != 0) {
        if (*it < 0) {
            throw std::runtime_error("Weights of weighed trees must be positive.");
        }
        it++;
    }
    if (pNode->m_Parent == NULL && it - itBegin < 3) {
        throw std::runtime_error("The root of a weighed tree must have at least 3 neighbors.)");
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



void WeighedTree::getDegrees(std::vector<int>& degrees) const{
    degrees.clear();
    getDegreesRecursive(degrees, m_Root);
    degrees[0]--;
    std::sort(degrees.begin(), degrees.end(), std::greater<int>());
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



