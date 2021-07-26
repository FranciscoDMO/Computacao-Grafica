#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>
#include <GL/glut.h>

#include "tinyxml/tinyxml2.h"
#include "vertex.h"

using namespace tinyxml2;
using namespace std;
using std::vector;

float alfa = 0.25f, beta = 0.5f, radius = 5.0f;
float camX, camY, camZ;

vector<Vertex*> models;

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

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    spherical2Cartesian();
    
    gluLookAt(camX,camY,camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    drawAxis();

    glBegin(GL_TRIANGLES);

    for(int i = 0; i < models.size(); i+=3){
        glColor3f(1,1,1);
        glVertex3f(models[i]->getX(),models[i]->getY(),models[i]->getZ());
        glVertex3f(models[i+1]->getX(),models[i+1]->getY(),models[i+1]->getZ());
        glVertex3f(models[i+2]->getX(),models[i+2]->getY(),models[i+2]->getZ());
    }

    glEnd();
    glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy){
    // put code to process special keys in here
    switch (key)
    {
    case GLUT_KEY_UP:
        beta += 0.25f;
        break;
    case GLUT_KEY_DOWN:
        beta -= 0.25f;
        break;
    case GLUT_KEY_LEFT:
        alfa -= 0.25f;
        break;
    case GLUT_KEY_RIGHT:
        alfa += 0.25f;
        break;
    }

    spherical2Cartesian();
    glutPostRedisplay();

}

void readFile(string fname) {
    string line;
    ifstream file(fname);

    if (file.fail()) {
        throw std::ios_base::failure(string("Couldn't find file: ") + fname);
    }

    else {
        while (getline(file, line)) {
            size_t pos;

            float x,y,z;

            x = std::stof(line,&pos);
            line.erase(0,pos+1);
            y = std::stof(line,&pos);
            line.erase(0,pos+1);
            z = std::stof(line,&pos);

            Vertex *v = new Vertex(x,y,z);
            models.push_back(v);
        }
    }
    file.close();
}

void readXML(string f_path){
    XMLDocument xmlDoc;
    XMLElement *element;

    if (!(xmlDoc.LoadFile(f_path.c_str()))) {

        element = xmlDoc.FirstChildElement();
        for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
            string ficheiro = element->Attribute("file");
            readFile(ficheiro);
        }
    }
    else {
        printf("XML File %s could not be found",f_path.c_str());
    }
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
        readXML(argv[1]);
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