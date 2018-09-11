#ifndef GRAPH_H
#define GRAPH_H

#include "nodeAndEdge.h"
#include <vector>
using namespace std;

template <class G>
class CNode;

template <class G>
class CEdge;

template <class N, class E>
class CGraph
{
public:
        typedef CGraph<N,E> self;
        typedef CNode<self> Node;
        typedef CEdge<self> Edge;
        typedef N n;
        typedef E e;

        vector<Node*> m_nodes;

        void generateNodes(int numNodes);
        void buildGraph();

        int getRandomNumber(int min, int max);

    //public:
        CGraph();
        ~CGraph();

        bool insertNode(N);
        bool removeNode(N);
        bool insertEdge(E,N,N);
        bool removeEdge(E,N,N);

        void printNodes();
        void printEdges();
        struct comparesTwoEdges{
            N current, final;
            comparesTwoEdges(N current, N final){
                this->current = current;
                this->final = final;
            }
            bool operator ()(Edge* edge1, Edge* edge2) {
                double w1 = (double)(edge1->m_data) + edge1->m_node[1]->m_data.calculateDistance(final);
                double w2 = (double)(edge2->m_data) + edge2->m_node[1]->m_data.calculateDistance(final);
                return w1 < w2;
            }

        };
        int pathBetweenNodes(N from, N To, vector<N> &vec);
        bool pathBetweenNodesRecursive(Node *, N, vector<Node*> &, int &);

        const vector<Node*> & getNodesList() const{
            return m_nodes;
        };

};

#endif // GRAPH_H
