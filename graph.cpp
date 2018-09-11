#include "graph.h"
#include "Point.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define TOTALNODES 50
#define MIN 0 // Lowest X or Y value of any Point
#define MAX 10 // Highest X or Y value of any Point
#define RADIO 1.5


template <class N,class E>
CGraph<N,E>::CGraph(){
    generateNodes( TOTALNODES );
    buildGraph();
}

template <class N,class E>
CGraph<N,E>::~CGraph(){}

template <class N,class E>
bool CGraph<N,E>::insertNode(N x)
{
	Node* tmp = new Node(x);
	m_nodes.push_back(tmp);
	cout << ">> [OK]: Node "<< x << " Insertion Successful " << endl;
	return 1;
}

template <class N,class E>
bool CGraph<N,E>::insertEdge(E x, N a, N b)
{
	Node* Na = nullptr; Node* Nb = nullptr;
	for(int i=0; i< m_nodes.size() ;i++){
		if(m_nodes[i]->m_data == a ) Na = m_nodes[i];
		if(m_nodes[i]->m_data == b ) Nb = m_nodes[i];
	}
	//poner condicion para que Na y Nb existan!!!!!!!!!!!!!!!
    if (Na == nullptr or Nb== nullptr ){
        cout << ">> [ERROR]: Cant remove Edge, check if exists nodes" << endl;
        return false;
    }

	Edge* tmp = new Edge(x,Na, Nb);
	Na->m_edges.push_back( new Edge(x,Na, Nb) );
    //if ( Na != Nb)
	//    Nb->m_edges.push_back( new Edge(x,Nb, Na) );

	cout << ">> [OK]: Edge "<< Na->m_data << " to "<< Nb->m_data << " Insertion Successful" << endl;
	return 1;
}

template <class N,class E>
bool CGraph<N,E>::removeNode(N x)
{
	Node* tmp = nullptr;
	for(int i=0; i< m_nodes.size(); i++){
		if(m_nodes[i]->m_data == x){
			tmp = m_nodes[i];
			for(int j=0; j<tmp->m_edges.size(); j++)
				removeEdge(tmp->m_edges[i]->m_data, tmp->m_edges[i]->m_node[0]->m_data, tmp->m_edges[i]->m_node[1]->m_data);

			cout << ">> [OK]: Node "<< tmp->m_data <<" Deleting ... " << endl;
			m_nodes.erase(m_nodes.begin()+i);
		}
	}

    printNodes();
	printEdges();
	return 1;
}

template <class N,class E>
bool CGraph<N,E>::removeEdge(E x, N a, N b)
{
	Node* Na; Node* Nb;
	for(int i=0; i< m_nodes.size() ;i++){
		if(m_nodes[i]->m_data == a ) Na = m_nodes[i];
		if(m_nodes[i]->m_data == b ) Nb = m_nodes[i];
	}
	Edge* borrar;
	//if(Na) // poner cond para que exista Na
	for(int i=0; i< Na->m_edges.size() ;i++)
		if(Na->m_edges[i]->m_data==x){
			borrar = Na->m_edges[i];
			Na->m_edges.erase(Na->m_edges.begin()+i);
		}

	delete borrar;
    cout << ">> [OK]: Removing edge: " + Na->m_data +" - "+ Nb->m_data << endl;
	printEdges();
	return 1;
}

template <class N,class E>
void CGraph<N,E>::generateNodes(int numNodes){
    int x, y;
    for(int i = 0 ; i < 20;i++){
        for(int j = 0 ; j < 20;j++){
            Point* newPoint = new Point();
            newPoint->setX( i );
            newPoint->setY( j );
            insertNode( *newPoint );
        }
    }
    /*
    for (int i = 0; i < numNodes ; ++i) {
        x = getRandomNumber( MIN, MAX);
        y = getRandomNumber( MIN , MAX);

        Point* newPoint = new Point();
        newPoint->setX( x );
        newPoint->setY( y );

        insertNode( *newPoint );
    }*/
};

template <class N,class E>
void CGraph<N,E>::buildGraph(){
    cout << "\n[OK]: Building graph " << endl;
    float distance;
    Point* point;
    Point* nextPoint;
    for (int i = 0; i < getNodesList().size() ; ++i) {
        point = &getNodesList()[i]->m_data  ;
        for (int j = 0; j < getNodesList().size() ; ++j) {
            nextPoint = & getNodesList()[j]->m_data;

            if ( i != j ){
                distance = point->calculateDistance( *nextPoint );
                //cout << "- " << distance << " | " << *point << " to " << *nextPoint << endl;
                if ( distance <= RADIO){
                    insertEdge(distance, *point, *nextPoint);
                }
            }

        }
    }
};

template <class N,class E>
int CGraph<N,E>::getRandomNumber(int min, int max) {
    int num,
    srand(time(NULL));
        num =  rand() % 50;
    return num;
};

template<class N, class E>
int CGraph<N,E>::pathBetweenNodes(N from, N To, vector<N> &vec){
    Node *fromNode = nullptr;
    Node* ToNode = nullptr;
    for(const auto &node: m_nodes){
        if(node->m_data == from) fromNode = node;
        if(node->m_data == To) ToNode = node;
    }
    vector<Node*>vec2;
    int costo = 0;
    this->pathBetweenNodesRecursive(fromNode, To, vec2, costo);
    vec2.push_back(fromNode);
    for(const auto &i: vec2)vec.push_back(i->m_data);
    return costo;
};

template<class N, class E>
bool CGraph<N,E>::pathBetweenNodesRecursive(Node* current, N To, vector<Node*> &vec, int& costo){
    if(current == nullptr) return false;
    if(current->m_data == To) {
        return true;
    }
    current->visited = true;
    vector<Edge*> vec2(current->m_edges);
    std::sort(vec2.begin(), vec2.end(), comparesTwoEdges(current->m_data, To));
    for(auto &edge: vec2){
    	if(!edge->m_node[1]->visited){
    	    if (pathBetweenNodesRecursive(edge->m_node[1], To, vec,costo)){
    	    	vec.push_back(edge->m_node[1]);
                costo += edge->m_data;
    	        return true;
    	    }
    	}
    }
    return false;
};

template <class N,class E>
void CGraph<N,E>::printNodes()
{
	cout << "\n Nodes List: ";
	for(int i=0; i<m_nodes.size(); i++)
		cout << m_nodes[i]->m_data << " - " ;
	cout << endl;
}


template <class N,class E>
void CGraph<N,E>::printEdges()
{
	cout << "\n Edges List: "<< endl;
	for(int i=0; i<m_nodes.size();i++){
	    cout << '[' << i << ']' << "--SIZE-->" << m_nodes[i]->m_edges.size() << endl;
		for(int j=0; j<m_nodes[i]->m_edges.size();j++){
			cout << " [ "<<i<<" ] "<< m_nodes[i]->m_edges[j]->m_node[0]->m_data << " to " << m_nodes[i]->m_edges[j]->m_node[1]->m_data
				 <<" with " << m_nodes[i]->m_edges[j]->m_data  << endl;
		}
	}

}


