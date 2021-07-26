#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>
#include <GL/glut.h>

#include "tinyxml/tinyxml2.h"
#include "vertex.h"
#include "parser.h"
#include "group.h"

using namespace tinyxml2;
using namespace std;
using std::vector;

float alfa = 0.25f, beta = 0.5f, radius = 40.0f;
float camX, camY, camZ;

Group* scene = new Group();

void changeSize(int w, int h) {

    if (h == 0)
        h = 1;

    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}
void spherical2Cartesian() {
    camX = radius * cos(beta) * sin(alfa);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alfa);
}

void drawAxis(){

    glBegin(GL_LINES);

    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);

    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);

    glEnd();
}

void renderGroup(Group* g){
	glPushMatrix();

	for(Oper* op: g->getOperations()){
		if(strcmp(op->getName(), "translate")==0){
			printf("%f %f %f\n", op->getX(),op->getY(),op->getZ());
			glTranslatef(op->getX(),op->getY(),op->getZ());
		}
		if(strcmp(op->getName(), "rotate")==0){
			glRotatef(op->getAngle(),op->getX(),op->getY(),op->getZ());
		}
		if(strcmp(op->getName(), "scale")==0){
			glScalef(op->getX(),op->getY(),op->getZ());
		}
	}

	glBegin(GL_TRIANGLES);

	for(Model* m: g->getModels()){
		std::vector<Vertex*> v = m->getVertices();

		for(int i = 0; i < v.size();i+=3){
		    glColor3f(1,1,1);
		    glVertex3f(v[i]->getX(),v[i]->getY(),v[i]->getZ());
		    glVertex3f(v[i+1]->getX(),v[i+1]->getY(),v[i+1]->getZ());
		    glVertex3f(v[i+2]->getX(),v[i+2]->getY(),v[i+2]->getZ());
		}
	}

	glEnd();

	for(Group* child: g->getChildren()){
		renderGroup(child);
	}

	glPopMatrix();
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    spherical2Cartesian();
    
    gluLookAt(camX,camY,camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

 	renderGroup(scene);

    glutSwapBuffers();
}


void processSpecialKeys(int key, int xx, int yy){
    // put code to process special keys in here
    switch (key)
    {
    case GLUT_KEY_UP:
        beta += 0.25f;
        if(beta > 1.2)
        	beta = 1.2;
        break;
    case GLUT_KEY_DOWN:
        beta -= 0.25f;
        if(beta < -1.2)
        	beta = -1.2;
        break;
    case GLUT_KEY_LEFT:
        alfa -= 0.25f;
        break;
    case GLUT_KEY_RIGHT:
        alfa += 0.25f;
        break;
    case GLUT_KEY_PAGE_DOWN: 
    	radius -= 1.0f;
		if (radius < 2.0f)
			radius = 2.0f;
		break;

	case GLUT_KEY_PAGE_UP: 
		radius += 1.0f; 
		break;
	}

    spherical2Cartesian();
    glutPostRedisplay();

}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("Engine - @CG2020");

    if(argc != 2){
        printf("Invalid input");
        return 0;
    }
    else {
        scene = readXML(argv[1]);
    }

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glutSpecialFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}