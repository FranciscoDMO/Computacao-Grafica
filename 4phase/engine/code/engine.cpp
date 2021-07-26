#ifdef _WIN32
#include "corecrt_math_defines.h"
#endif
#include <stdlib.h>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>
#include <GL/glew.h>


#include <GL/glut.h>

#include "tinyxml/tinyxml2.h"
#include "vertex.h"
#include "parser.h"
#include "group.h"


using namespace tinyxml2;
using namespace std;
using std::vector;

float alpha = 0.25f, beta = 0.5f, r = 40.0f;
float camX, camY, camZ;

#define ANG2RAD M_PI/180.0 

float userX = 00, userZ = 0;
float lookX = 1.0, lookZ = 0.0f;
int startX, startY, tracking = 0;

float eyeHeight = 10.0f;

float rc = 10.f;


Group* scene = new Group();

std::vector<Translate*> orbits;

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

void renderGroup(Group* g){
	glPushMatrix();

    

    for(Light* l: g->getLight()){
        l->renderLight();
    }
    
	for(Oper* op: g->getOperations()){
		if(strcmp(op->getName(), "translate")==0){
			((Translate*) op)->draw();
		}
		if(strcmp(op->getName(), "rotate")==0){
			((Rotate*)op)->draw();
		}
		if(strcmp(op->getName(), "scale")==0){
			glScalef(op->getX(),op->getY(),op->getZ());
		}
	}

	glBegin(GL_TRIANGLES);

	for(Model* m: g->getModels()){
        m->renderModel();
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

    float Px = userX;
    float Pz = userZ;
    float Py = eyeHeight;
    float Ly = Py;
    float Lx = lookX;
    float Lz = lookZ;
    
    gluLookAt(Px, Py, Pz,
        Lx, Ly, Lz,
        0.0f, 1.0f, 0.0f);

 	renderGroup(scene);

    for (int i = 0; i < orbits.size(); i++){
        orbits[i]->drawCurve();
    }

    glutSwapBuffers();
}

void crossProduct(float vect_A[], float vect_B[], float cross_P[])
{
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

void processSpecialKeys(int key, int xx, int yy) {

    float dX = lookX - userX;
    float dZ = lookZ - userZ;
    float d[] = { dX, 0, dZ };
    float up[] = { 0.0f, 1.0f, 0.0f };

    float r[] = { 0, 0, 0 };
    crossProduct(d, up, r);

    // Normalize r
    float len = sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    r[0] = r[0] / len;
    r[1] = r[1] / len;
    r[2] = r[2] / len;

    // Normalize d

    float lenD = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    d[0] = d[0] / lenD;
    d[1] = d[1] / lenD;
    d[2] = d[2] / lenD;

    float side = 0.0f;
    float forward = 0.0f;

    switch (key) {

    case GLUT_KEY_RIGHT:
        side = 2.0f;
        break;

    case GLUT_KEY_LEFT:
        side = -2.0f;
        break;

    case GLUT_KEY_UP:
        forward = 2.0f;
        break;

    case GLUT_KEY_DOWN:
        forward = -2.0f;
        break;
    }

    userX = userX + side * r[0];
    userZ = userZ + side * r[2];

    lookX = lookX + side * r[0];
    lookZ = lookZ + side * r[2];

    userX = userX + forward * d[0];
    userZ = userZ + forward * d[2];

    lookX = lookX + forward * d[0];
    lookZ = lookZ + forward * d[2];

    glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) {

    if (state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy) {

    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {
        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }

    lookX = userX + rAux * sin(alphaAux * ANG2RAD);
    lookZ = userZ + rAux * cos(alphaAux * ANG2RAD);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1080,720);
    glutCreateWindow("Engine - @CG2020");
    glewInit();
    ilInit();

    if(argc != 2){
        printf("Invalid input");
        return 0;
    }
    else {
        scene = readXML(argv[1], &orbits);
    }

    glutIdleFunc(renderScene);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);
    glEnable(GL_LIGHT7);

    glEnable(GL_TEXTURE_2D);

    glutMainLoop();

    return 1;
}