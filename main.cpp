#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#define KEY_ESC 27
#include <stdlib.h>
#include<time.h>
#include "graph.cpp"
#include "Point.h"
#include "nodeAndEdge.h"
#define REDIM 25



CGraph<Point, float> graph;
vector<Point>ans;
Point A,B;

using namespace std;

void OnMouseClick(int button, int state, int x, int y)
{
    int n_x;
    int n_y;
    Point* point;
    int option;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    n_x = x/(REDIM-1);
    n_y = (600-y)/(REDIM-1);
    cout<<x/(REDIM-1)<<" , "<<(600-y)/(REDIM-1)<<endl;
    cout<<"1.- nodo inicio"<<endl;
    cout<<"2.- nodo fin"<<endl;
    cout<<"3.- encontrar camino"<<endl;
    cout<<"4.- borrar nodo"<<endl;
    cin>>option;

    if(option == 1){

        A.setX(n_x);A.setY(n_y);

    }else if(option == 2){

        B.setX(n_x);B.setY(n_y);

    }else if(option == 3){
        if(ans.size()> 0){
            ans.clear();
        }
        float distance = graph.pathBetweenNodes(A,B,ans);
        for(auto i = static_cast<int>(ans.size() - 1); i >= 0; --i ){
            cout << ans[i].getX() << ", " << ans[i].getY() << " -> ";
        }
        cout << "//" << endl;
        cout << "COSTO DEL RECORRIDO: " << distance << endl;

    }else if(option == 4){

        for(int i = 0 ; i < graph.getNodesList().size() ;i++){
            point = &(graph.getNodesList())[i]->m_data;
            if ( (*point).getX() == n_x && (*point).getY() == n_y){
                graph.removeNode(*point);
                //graph.removeNode((graph.getNodesList())[i]);
            }

        }
    }


  }
}

void resize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);//matriz de proyeccion
    glLoadIdentity();//cargamos la matriz identidad
    glOrtho(0,600,0,600,-300,300);
    glMatrixMode(GL_MODELVIEW);//matriz de modelado
    glLoadIdentity();//matriz identidad
    glEnable(GL_DEPTH_TEST);//activamos el Test de profundidad
    glDepthFunc(GL_LEQUAL);//funcion de borrrado buffer depth

}
void nodos()
{
    int x;
    int y;
    Point* point;
    Point* pointRec;
    glPointSize(10);
    glBegin(GL_POINTS);
        for(int i = 0 ; i < graph.getNodesList().size() ;i++){
            point = &(graph.getNodesList())[i]->m_data;
            glColor3f(1,1,1);
            glVertex3f((*point).getX()*REDIM,(*point).getY()*REDIM,0);
        }
        for(int i = 0; i < ans.size();i++){
            pointRec = &ans[i];
            glColor3f(0,0,1);
            glVertex3f((*pointRec).getX()*REDIM,(*pointRec).getY()*REDIM,0);
        }
    glEnd();

}

void edges()
{
    int x;
    int y;
    int i , j;
    Point* point;
    Point* point2;
    glBegin(GL_LINES);
        for(i = 0 ; i < graph.getNodesList().size() ;i++){
            point = &(graph.getNodesList())[i]->m_data;
            for ( j = 0 ; j <  ((graph.getNodesList())[i]->getEdges().size()) ;j++){
                point2 = &(graph.getNodesList())[i]->getEdges()[j]->m_node[1]->m_data;
                glColor3d(255,0,0);
                glVertex2d((*point).getX()*REDIM,(*point).getY()*REDIM);
                glVertex2d((*point2).getX()*REDIM,(*point2).getY()*REDIM);

               // cout<< "("<<(*point).getX()*10 << " , "<< (*point).getY()*10<<")"<< "("<<(*point2).getX()*10 << " , "<< (*point2).getY()*10<<")"<<endl;
            }
        }
        //cout<<i <<" "<<j;
    glEnd();


}

void display()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();
     //gluLookAt(1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
     nodos();
     edges();
     glutSwapBuffers();
}



void key(unsigned char key, int x, int y)    //funcion para uso del teclado
{
    switch(key)
    {
          case 27 :
            exit(0);
            break;
        default:
            break;
    }

glutPostRedisplay();    //redibuja el display
}

void idle()
{
    glutPostRedisplay();//redibuja el display
}




int main(int argc, char *argv[])
{

    /*cout << "GRAPH NODES" << endl;
	graph.printNodes();
	cout << "GRAPH EDGES" << endl;
	graph.printEdges();
	cout << "Insert Nodes to find path" << endl;
	int a,b,c,d;
    //1 double distance = 0f;
	cin >> a >> b >> c >> d;
	Point A,B;
	A.setX(a);A.setY(b);
	B.setX(c);B.setY(d);

	float distance = graph.pathBetweenNodes(A,B,ans);
	for(auto i = static_cast<int>(ans.size() - 1); i >= 0; --i ){
		cout << ans[i].getX() << ", " << ans[i].getY() << " -> ";
	}
	cout << "//" << endl;
    cout << "COSTO DEL RECORRIDO: " << distance << endl;

*/
    glutInit(&argc, argv);//inicializacion del glut
    glutInitWindowSize(600,600);//tama�o de la ventana
    glutInitWindowPosition(400,100);//posicion de la ventana en el monitor
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//modo de visualizacion inicial
    glutCreateWindow("Malla grafo");//nombre de la ventana
    glClearColor(0,0,0,0);//especificar valores claros para el color del buffer
    glutReshapeFunc(resize);//control de ventanas
    glutDisplayFunc(display);//establece la devoluci�n de llamada de visualizaci�n de la ventana actual
    glutKeyboardFunc(key);//funcion teclado especial
    //glutFullScreen();
    glutMouseFunc(&OnMouseClick);
    glutIdleFunc(idle);//funcion de animacion

   // char soundfile[20] ="C:\planeta.wav";//sonido
    //PlaySound((LPCSTR)soundfile, NULL, SND_FILENAME | SND_ASYNC );//sonido

    glutMainLoop();//bucle de procesamiento de eventos de GLUT

    return 0;
}
