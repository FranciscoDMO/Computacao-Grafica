#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include <string>
#include <cmath>
#include <GL/glut.h>
#include <IL/il.h>
#include "vertex.h"

using namespace std;
using std::vector;

class Model{
    std::vector<Vertex*> vertices;
    float buff_size[3];
    GLuint texture, buffers[3];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float ambient[4];
    float shininess;


public:
    Model();
    Model(std::vector<Vertex*> vertices);
    std::vector<Vertex*> getVertices();
    void setUpVBO(std::vector<Vertex*> vert, std::vector<Vertex*> norm, std::vector<Vertex*> tex, std::vector<float> ind);
    void setUpTexture(string file);
    void pushMaterials(float* d, float* s, float* e, float* a, float shine);
    void renderModel();
    ~Model(void);
};

class Oper{
    char* opName;
    float x,y,z;

public:
    Oper();
    Oper(char* opName, float x, float y, float z);
    char* getName();
    float getX();
    float getY();
    float getZ();
    void draw();
};

class Translate: public Oper{
    float time;
    std::vector<Vertex*> curvePoints;
    std::vector<Vertex*> catmullPoints;

public:
    Translate();
    Translate(char* opName, float x, float y, float z, float time);
    void addPoint(Vertex* v);
    void generateCurve();
    void drawCurve();
    int getCurveSize();
    void draw();
};

class Rotate: public Oper{
    float angle;
    float time;

public:
    Rotate();
    Rotate(char* opName, float x, float y, float z, float angle, float time);
    void draw();
};

class Light {
    float pos[4];

public:
    Light(float *p);
    void renderLight();
};

class Group {
    std::vector<Oper*> operations;
    std::vector<Model*> models;
    std::vector<Group*> children;
    std::vector<Light*> lights;

public:
    Group();
    Group(std::vector<Oper*> operations, std::vector<Model*> model, std::vector<Group*> children);
    std::vector<Oper*> getOperations();
    std::vector<Model*> getModels();
    std::vector<Group*> getChildren();
    std::vector<Light*> getLight();
    void pushModel(Model* m);
    void pushOperation(Oper* op);
    void pushChild(Group* child);
    void pushLight(Light* l);
    ~Group(void);
};

#endif