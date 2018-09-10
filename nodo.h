#define eps 1e-4
#define Vector Punto
//Punto y propiedades basicas
struct Punto{
	double x=0.0,y=0.0;
	Punto(){}
	Punto(double a,double b){
		x=a;
		y=b;
	}
	Punto operator =(const Punto &B){
		x=B.x;
		y=B.y;
		return *(this);
	}
	Punto operator +=(const Punto &B){
		x+=B.x;
		y+=B.y;
		return *(this);
	}
	bool operator < (const Punto B)const{
		return (x==B.x)?y<B.y:x<B.x;
	}
	bool operator ==(const Punto B)const{
		return (x==B.x && y==B.y);
	}
	double norm()const{
		return sqrt(x*x+y*y);
	}
};

Punto operator +(const Punto &a, const Punto &b){
	return Punto(a.x + b.x, a.y + b.y);
}

Punto operator -(const Punto &a, const Punto &b){
	return Punto(a.x - b.x, a.y - b.y);
}

Punto operator *(const Punto &a,double k){
	return Punto(a.x*k,a.y*k);
}

double prCruz(const Vector &A,const Vector &B){
	return A.x * B.y - A.y * B.x;
}

bool esParalelo(const Vector &A,const Vector &B){
	return abs(prCruz(A,B))<eps;
}

Punto interseccionDeRecta(const Punto &A, const Punto &B,const Punto &C, const Punto &D){
	return A+ (B-A) * (prCruz(C-A,D-C)/prCruz(B-A,D-C));
}



struct Nodo{
	int ID;
	Punto punto;//coordenada
	map<int,Nodo*> aristas;//nodos vecinos
	int obstaculos=0;
	Nodo (Punto B,int id=0){
		ID=id;
		punto=B;
	}
	bool operator <(const Nodo B)const{
		return punto<B.punto;
	}
	bool insertarArista(Nodo* B){
		if(B==NULL){
			return false;
		}
		auto it = aristas.find(B->ID);
		if(it==aristas.end()){
			aristas[B->ID]=B;
			return true;
		}
		return false;
	}
};
