#include "group.h"

using std::vector;

//Construtores 

//Model
Model::Model(){
}

Model::Model(vector<Vertex*> v){
	vertices=v;
}

vector<Vertex*> Model::getVertices(){
	return vertices;
}

//Operations
Oper::Oper(){
}

Oper::Oper(char* name, float x_var, float y_var, float z_var, float angle_var){
	opName = name;
	x = x_var;
	y = y_var;
	z = z_var;
	angle = angle_var;
}

char* Oper::getName(){
	return opName;
}

float Oper::getX(){
	return x;
}
float Oper::getY(){
	return y;
}
float Oper::getZ(){
	return z;
}
float Oper::getAngle(){
	return angle;
}

//Group
Group::Group(){
}

Group::Group(vector<Oper*> op, vector<Model*> m, vector<Group*> c){
    operations = op;
    models = m;
    children = c;
}

vector<Oper*> Group::getOperations(){
    return operations;
}

vector<Model*> Group::getModels(){
    return models;
}

vector<Group*> Group::getChildren(){
    return children;
}

void Group::pushOperation(Oper* o){
    operations.push_back(o);
}

void Group::pushModel(Model* m){
    models.push_back(m);
}

void Group::pushChild(Group* c){
    children.push_back(c);
}
