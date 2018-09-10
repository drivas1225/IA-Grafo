#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#define KEY_ESC 27
#include <GL/glut.h>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <time.h>

using namespace std;


#include "grafo.h"

Grafo grafo;

struct Camara{
	bool opcionHeuristica=false;
	double WIDTH = 850;
	double HEIGHT = 850;
	double rotate = 1;
	Punto CENTER;
	bool transform=true;
	Camara(){};
	Camara(double w,double h){
		WIDTH=w;
		HEIGHT=h;
	}
	double ASPECT(){
		return WIDTH/HEIGHT;
	}
	void zoom(double Scale){
		rotate*=Scale;
		transform=true;
	}
	void move(double inX,double inY){
		CENTER+=Punto(inX,inY);
	}
};


Camara CAMARA(850,850);

//Controlar la escena con el teclado
void keyPress(int key,int x,int y){
	if(key==KEY_ESC){//salir
		exit(0);
	}
	CAMARA.transform=true;
	if(key==(int)'+'){//zoom in
		CAMARA.zoom(2);
	} else if(key==(int)'-'){//zoom out
		CAMARA.zoom(.5);
	} else if(key==GLUT_KEY_RIGHT){//mover derecha
		CAMARA.move(-15*1/CAMARA.rotate,0);
	} else if(key==GLUT_KEY_LEFT){//mover izquierda
		CAMARA.move(15*1/CAMARA.rotate,0);
	} else if(key==GLUT_KEY_UP){//mover arriba
		CAMARA.move(0,-15*1/CAMARA.rotate);
	} else if(key==GLUT_KEY_DOWN){//mover abajo
		CAMARA.move(0,15*1/CAMARA.rotate);
	} else if(key==GLUT_KEY_F1){
		CAMARA.opcionHeuristica=false;
	} else if(key==GLUT_KEY_F2){
		CAMARA.opcionHeuristica=true;
	} else {
		CAMARA.transform=false;
	}
	if(CAMARA.transform)
        glutPostRedisplay();
}

//Redimencionar escena de acuerdo a el tamaño de la ventana
void reshapeScene(GLint width, GLint height){
	CAMARA.transform=true;
	if (height < width){
		double w = (double)height;
		double left = (width - w) / 2;
		glViewport(left, 0, w, height);
	} else {
		double h=(double)width;
		double floor = (height-h)/2;
		glViewport(0,floor,width,h);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-CAMARA.WIDTH/2,CAMARA.WIDTH/2,-CAMARA.HEIGHT/2,CAMARA.HEIGHT/2);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void graficarArista(Punto A, Punto B,bool obstaculo=false){
	glVertex2f(A.x+CAMARA.CENTER.x,A.y+CAMARA.CENTER.y);
	glVertex2f(B.x+CAMARA.CENTER.x,B.y+CAMARA.CENTER.y);
}

void graficarArista(Nodo* A, Nodo* B,bool obstaculo=false){
	graficarArista((*A).punto,(*B).punto, obstaculo);
}

void graficoProfundidad(Nodo* u,set<pair<int,int> >& Obstaculos,set<int>& S,int padreID=-1){
	S.insert(u->ID);
	for (auto it=(*u).aristas.begin();it!=(*u).aristas.end();it++){
		glColor3f(1.0f,1.0f,0);
		if(S.find(it->first)==S.end()){
			if(Obstaculos.find(make_pair(u->ID,it->first))!=Obstaculos.end()){
				glColor3f(1.0f,0.5f,0);
			}
			graficarArista(u,it->second);
			graficoProfundidad(it->second,Obstaculos,S,u->ID);
		} else if((it->second)->ID!=padreID){
			if(Obstaculos.find(make_pair(u->ID,it->first))!=Obstaculos.end()){
				glColor3f(1.0f,0.5f,0);
			}
			graficarArista(u,it->second);
		}
	}
}

void graficarAristasDeGrafo(Grafo* G){
	glBegin(GL_LINES);
        glColor3f(1.0f,1.0f,0);
		set<int> S;
		if(G->size){
			graficoProfundidad((G->nodos.begin())->second,G->Obstaculos,S);
		}
	glEnd();
}


long long xorshift_x, xorshift_y, xorshift_z,xorshift_w;

void SeedXorShift (){
  srand(time(NULL));
  time_t timer= time(NULL);
  struct tm y2k ;
  y2k.tm_hour = 0;
  y2k.tm_min = 0;
  y2k.tm_sec = 0;
  y2k.tm_year = 100;
  y2k.tm_mon = 0;
  y2k.tm_mday = 1;
  long long seconds= difftime(timer ,mktime(&y2k));//diferencia desde 1970
  xorshift_x = seconds;
  xorshift_y = seconds ^ 123456789;//operacion xor
  xorshift_z=seconds << 31;//shift left
  xorshift_w=seconds >> 11;//shift rigth
}

long long XorShift (long long MAX_XOR){
  long long tmp;
  tmp=xorshift_x ^( xorshift_x <<11);//xor
  xorshift_x=xorshift_y ;
  xorshift_y=xorshift_z ;
  xorshift_z=xorshift_w ;
  xorshift_w=xorshift_w ^ ( xorshift_w >>19)^(tmp^(tmp>>8)) ;//xor y shiftrigth
  return xorshift_w % MAX_XOR;//modulo
}


void generarPuntos(set<Punto>& P,int n){
	while(P.size()<n){
		double x=XorShift((long long)CAMARA.WIDTH);
		double y=XorShift((long long)CAMARA.HEIGHT);
		P.insert(Punto(x,y));
	}
}

void crearGrafo(int n, Grafo* G){
	set<Punto> P;
	generarPuntos(P,n);
	vector<Punto> V;
	for (auto it=P.begin();it!=P.end();it++){
		V.push_back(Punto(it->x-CAMARA.WIDTH/2,it->y-CAMARA.HEIGHT/2));
	}
	delaunayTriangulation(V,G);
}



void renderScene(void){
	if(CAMARA.transform){ // Si se sufrio una transformación (eg. ajuste de la ventana, movimiento, zoom)
		gluOrtho2D(-CAMARA.WIDTH/2,CAMARA.WIDTH/2,-CAMARA.HEIGHT/2,CAMARA.HEIGHT/2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Borrar datos de la escena
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(CAMARA.rotate,CAMARA.rotate,1.0f);//Realizar transformación de escala (zoom_in(x2), zoom_out(x.5))
		graficarAristasDeGrafo(&grafo);//Graficar la escena con la traslación de camara
		if(!CAMARA.opcionHeuristica)
			graficarCaminoDeGrafoProfundidad(&grafo,0,grafo.size-1);
		else
			graficarCaminoMejorElPrimero(&grafo,0,grafo.size-1);
		CAMARA.transform=false;//Se realizo todas las transformaciones
		glutSwapBuffers();
	}
}

void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

int main(int argc, char **argv){
	cout << "Numero de Nodos: ";
	int n;
	cin >> n;
	crearGrafo(n,&grafo);


	//Iniciando propiedades de la ventana
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-CAMARA.WIDTH)/2, (glutGet(GLUT_SCREEN_HEIGHT)-CAMARA.HEIGHT)/2);
	glutInitWindowSize(CAMARA.WIDTH, CAMARA.HEIGHT);
	glutCreateWindow("Busqueda");
	//init_GL();

	//Iniciando funciones de interacción
	glutDisplayFunc(renderScene);
	glutSpecialFunc(keyPress);
	glutReshapeFunc(reshapeScene);

	//start animation
	glutMainLoop();

	return 0;
}
