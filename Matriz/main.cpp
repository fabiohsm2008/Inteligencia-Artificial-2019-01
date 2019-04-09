#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <utility>
#include <algorithm>
#include <queue>
//#include <quadTree.h>
#define KEY_ESC 27

using namespace std;

//Crear quad tree

int a = 1;
int inicio, fin;

struct point{
    float x,y;
    point(float a, float b):x(a),y(b){}
    point(){}
};

struct line{
    point u,v;
    line(point a, point b):u(a),v(b){}
    line(){}
};

float euclidean(point u, point v){
    return sqrt(pow(u.x-v.x,2)+pow(u.y-v.y,2));
}

vector<point> points;
vector<line> lines;
vector<vector<float> > graph(500);
vector<point> dfs;
vector<point> bfs;
vector<point> astar;

vector<int> vecinos(int punto){
    vector<int> rpta;
    for(int i = 0; i < graph[punto].size(); ++i){
        if(graph[punto][i]>0)
            rpta.push_back(i);
    }
    return rpta;
}

vector<point> ciega_profundidad(){
    vector<point> rpta;
    vector<bool> visitados;
    vector<int> recorrido;
    visitados.assign(points.size(), 0);
    recorrido.assign(points.size(), -1);
    vector<int> iniciales;
    iniciales.push_back(inicio);
    while(!iniciales.empty()){
        int valor = iniciales[0];
        iniciales.erase(iniciales.begin());
        if(visitados[valor]){
            continue;
        }
        visitados[valor] = 1;
        if(valor == fin){
            int val = fin;
            while(recorrido[val] != -1){
                rpta.push_back(points[val]);
                val = recorrido[val];
            }
            rpta.push_back(points[val]);
            reverse(rpta.begin(),rpta.end());
            return rpta;
        }
        vector<int> temp = vecinos(valor);
        for(int i = 0; i < temp.size(); i++){
            if(!visitados[temp[i]]){
                recorrido[temp[i]] = valor;
            }
        }
        iniciales.insert(iniciales.begin(), temp.begin(), temp.end());
    }
    cout << "No hay camino" << endl;

    return rpta;
}

vector<point> ciega_amplitud(){
    vector<point> rpta;
    vector<bool> visitados;
    vector<int> recorrido;
    visitados.assign(points.size(), 0);
    recorrido.assign(points.size(), -1);
    vector<int> iniciales;
    iniciales.push_back(inicio);
    while(!iniciales.empty()){
        int valor = iniciales[0];
        iniciales.erase(iniciales.begin());
        if(visitados[valor]){
            continue;
        }
        visitados[valor] = 1;
        if(valor == fin){
            int val = fin;
            while(recorrido[val] != -1){
                rpta.push_back(points[val]);
                val = recorrido[val];
            }
            rpta.push_back(points[val]);
            reverse(rpta.begin(),rpta.end());
            return rpta;
        }
        vector<int> temp = vecinos(valor);
        for(int i = 0; i < temp.size(); i++){
            if(!visitados[temp[i]]){
                recorrido[temp[i]] = valor;
            }
        }
        iniciales.insert(iniciales.end(), temp.begin(), temp.end());
    }
    cout << "No hay camino" << endl;

    return rpta;
}

class comparation2{
public:
    bool operator ()(pair<int,float> u,pair<int,float> v){
        return u.second>v.second;
    }
};

vector<point> a_star(){
    vector<point> camino;
    vector<int> neightboors;
    vector<bool> flag(500,true);
    vector<float> distancia(500,1000000);
    vector<int> prev(500,-1);
    int s = inicio, vecino;
    prev[inicio]=inicio;
    distancia[s]=0;
    float aux;
    priority_queue<pair<int,float>,vector<pair<int,float> >, comparation2> options;
    while(s!=fin){
        //flag[s]=false;
        neightboors=vecinos(s);
        for(size_t i=0;i<neightboors.size();++i){
            vecino=neightboors[i];
            if(flag[vecino]){
                aux= distancia[s]+graph[s][vecino];
                if(aux < distancia[vecino]){
                    prev[vecino]=s;
                    distancia[vecino]=aux;
                    options.push(make_pair(vecino,aux+euclidean(points[vecino],points[fin])));
                }
            }
        }
        if(options.empty())
            break;
        //camino.push_back(points[options.top().first]);
        s=options.top().first;
        options.pop();
    }
    if(s!=fin){
        cout<<"No hay camino"<<endl;
        return camino;
    }
    while(prev[s]!=s){
        camino.push_back(points[s]);
        s=prev[s];
    }
    camino.push_back(points[inicio]);
    return camino;
}

//dibuja un simple gizmo
void displayGizmo()
{
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glColor3d(255,255,255);
        glVertex2d(3,3);
        glVertex2d(3,97);
        glVertex2d(97,97);
        glVertex2d(97,3);
    glEnd();
    glLineWidth(1.0f);
	glBegin(GL_LINES);
        glColor3d(0,0,255);
        for(size_t i=0;i<lines.size();++i){
            glVertex2d(lines[i].u.x,lines[i].u.y);
            glVertex2d(lines[i].v.x,lines[i].v.y);
        }
	glEnd();
    glPointSize(3.0f);
	glBegin(GL_POINTS);
        glColor3d(255,0,0);
        for(size_t i=0;i<points.size();++i)
            glVertex2d(points[i].x, points[i].y);
	glEnd();
	glPointSize(6.0f);
	glBegin(GL_POINTS);
        glColor3d(0,255,0);
        glVertex2d(points[inicio].x,points[inicio].y);
        glVertex2d(points[fin].x,points[fin].y);
    glEnd();
}
bool r = false;

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    //convertir x,y
	//insertar un nuevo punto en el quadtree
	std::cout << x-300 << " " << 300-y << std::endl;
  }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
    r = true;
  }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
    r = false;
  }
}

void OnMouseMotion(int x, int y)
{
    if(!r)
        cout << x-300 << " " << 300-y << std::endl;
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}

void pintar_ciega(){
    if(a == 1){
        glBegin(GL_LINE_STRIP);
        glColor3d(255.0,255.0,255.0);
        for(int i = 0; i < dfs.size(); ++i){
            glVertex2d(dfs[i].x, dfs[i].y);
        }
        glEnd();
    }
    else if(a == 2){
        glBegin(GL_LINE_STRIP);
        glColor3d(255.0,255.0,255.0);
        for(int i = 0; i < bfs.size(); ++i){
            glVertex2d(bfs[i].x, bfs[i].y);
        }
        glEnd();
    }
    else if(a == 3){
        glBegin(GL_LINE_STRIP);
        glColor3d(255.0,255.0,255.0);
        for(int i = 0; i < astar.size(); ++i){
            glVertex2d(astar[i].x, astar[i].y);
        }
        glEnd();
    }
}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(0.0f,  100.0f,0.0f, 100.0f, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())

    /*glBegin(GL_POINTS);
    for()
        glVertex2f(v.at(i)[0],v.at(i)[1])
    glEnd();*/
	//dibuja el gizmo
	displayGizmo();
	pintar_ciega();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}

GLvoid teclas(int key, int x, int y){
    switch (key) {
	case GLUT_KEY_RIGHT:
            a=1;

		break;
    case GLUT_KEY_LEFT:
            a=2;

        break;
    case GLUT_KEY_UP:
            a=3;

        break;
	default:
		break;
	}
}

vector<point> generate_points(int num){
    vector<point> result(num);
    for(int i=0;i <num;++i){
        result[i].x=rand()%91+5;
        result[i].y=rand()%91+5;
    }
    return result;
}

class comparation{
public:
    bool operator ()(pair<int,float> u,pair<int,float> v){
        return u.second<v.second;
    }
};

void generate_lines(int num){
    int unions=2;
    cout<<unions<<endl;
    for(int i=0;i<num;++i){
        vector< pair<int,float> > aux;
        for(int j=0;j<num;++j){
            if(i!=j){
                aux.push_back(make_pair(j,euclidean(points[i],points[j])));
            }
        }
        sort(aux.begin(),aux.end(),comparation());
        for(int j=0;j<unions;++j){
            graph[i][aux[j].first]=aux[j].second;
            graph[aux[j].first][i]=aux[j].second;
        }
    }
}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	srand(time(NULL));
	for(int i=0;i<500;++i){
        graph[i].assign(500,0);
	}
    points = generate_points(500);
    generate_lines(500);
    for(int i=0;i<500;++i){
        for(int j=0;j<500;++j){
            if(graph[i][j]>0)
                lines.push_back(line(point(points[i]),point(points[j])));
        }
    }

    cout << "Ingrese el Inicio: ";
    cin >> inicio;
    cout << "Ingrese el Final: ";
    cin >> fin;

    dfs = ciega_profundidad();
    bfs = ciega_amplitud();
    astar= a_star();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("QuadTree"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutSpecialFunc(&teclas);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
