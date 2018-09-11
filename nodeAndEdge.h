#ifndef NODE_EDGED_H
#define NODE_EDGED_H
#include <vector>
using namespace std;

template <class G>
class CNode
{
public:
	typedef typename G::Edge Edge; // typedef typename G::Edge Edge; // tmbien es valido esto porque en la clase grafo, decimos que ambos son sus
	typedef typename G::n N;
	typedef typename G::e E;
	typedef typename G::Node Node;
	CNode(N x){
		m_data = x;
		this->visited = false;
	}
	~CNode();
	bool visited;
	N m_data;
	vector<Edge*> m_edges;

	N getData() const{
		return m_data;
	};

	const vector<Edge *> & getEdges() const{
		return m_edges;
	};
};

template <class G>
CNode<G>::~CNode(){}


template <class G>
class CEdge
{
public:
	typedef typename G::Node Node;
	typedef typename G::e E;

	CEdge(E x, Node* a, Node* b){
		m_data = x;
		m_node[0] = a ;
		m_node[1] = b ;
	}
	~CEdge();

	/* data */
	Node* m_node[2];
	E m_data;
};


template <class G>
CEdge<G>::~CEdge()
{
	//delete m_node;
}

#endif // NODE_EDGED_H
