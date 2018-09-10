#include "nodo.h"

struct Grafo{
	int size=0;
	map <Punto,int> IDs;
	map<int,Nodo*>nodos;
	set< pair<int,int> > Obstaculos;
	Grafo(){}
	Nodo* insertarNodo(Punto B){
		auto it = IDs.find(B);
		if(it!=IDs.end()){
			return nodos[it->second];
		}
		Nodo* nuevoNodo=new Nodo(B,size++);
		IDs[B]=size-1;
		nodos[size-1]=nuevoNodo;
		return nuevoNodo;
	}
	int aristasNodo(Punto A){
		if (IDs.find(A)==IDs.end()){
			return 0;
		}
		return nodos[IDs[A]]->aristas.size()-nodos[IDs[A]]->obstaculos;
	}
	bool insertarArista(Punto A,Punto B,bool obstaculo=0){
		Nodo* pNodo=insertarNodo(A);
		Nodo* sNodo=insertarNodo(B);
		if(obstaculo){
			Obstaculos.insert(make_pair(pNodo->ID,sNodo->ID));
			Obstaculos.insert(make_pair(sNodo->ID,pNodo->ID));
			nodos[IDs[A]]->obstaculos++;
			nodos[IDs[B]]->obstaculos++;
		}
		return ((*pNodo).insertarArista(sNodo) && (*sNodo).insertarArista(pNodo));
	}

	bool mejorElPrimero(int begin,int end, vector<pair<int,int> >& visitados,vector<int>& Anterior){
		Anterior.resize(size);
		Anterior[begin]=-1;
		Anterior[end]=-1;
		priority_queue<pair<double,int>,vector<pair<double,int> >,greater<pair<double,int> > > Q;
		Q.push(make_pair((nodos[begin]->punto-nodos[end]->punto).norm(),begin));
		bool Visitado[size];
		for (int i=0;i<size;i++){
			Visitado[i]=0;
		}
		Visitado[begin]=1;
		while(!Q.empty()){
			pair<long,int> p=Q.top();
			Q.pop();
			int here=p.second;
			if(here==end){
				return true;
			}
			for (auto it=nodos[here]->aristas.begin();it!=nodos[here]->aristas.end();it++){
				if(!Visitado[it->first]){
					if(Obstaculos.find(make_pair(here,it->first))==Obstaculos.end()){
						Visitado[it->first]=1;
						Anterior[it->first]=here;
						visitados.push_back(make_pair(here,it->first));
						Q.push(make_pair(((it->second)->punto-nodos[end]->punto).norm(),it->first));
					}
				}
			}
		}
		return false;
	}
};

