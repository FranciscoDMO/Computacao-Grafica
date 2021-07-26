#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include <string>
#include <GL/glut.h>
#include "vertex.h"

using namespace std;
using std::vector;

class Model{
    std::vector<Vertex*> vertices;

public:
    Model();
    Model(std::vector<Vertex*> vertices);
    std::vector<Vertex*> getVertices();
};

class Oper{
    char* opName;
    float x,y,z,angle;

public:
    Oper();
    Oper(char* opName, float x, float y, float z, float angle);
    char* getName();
    float getX();
    float getY();
    float getZ();
    float getAngle();
};

class Group {
    std::vector<Oper*> operations;
    std::vector<Model*> models;
    std::vector<Group*> children;

public:
    Group();
    Group(std::vector<Oper*> operations, std::vector<Model*> model, std::vector<Group*> children);
    std::vector<Oper*> getOperations();
    std::vector<Model*> getModels();
    std::vector<Group*> getChildren();
    void pushModel(Model* m);
    void pushOperation(Oper* op);
    void pushChild(Group* child);
};

#endif