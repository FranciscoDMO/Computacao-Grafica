#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include "group.h"
#include <iostream>


using std::vector;


void catmullRomPoint(float t, float *p, std::vector<Vertex*> points);
void catmullCalculate(float t, int* index, float* p, std::vector<Vertex*> points);

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

Oper::Oper(char* name, float x_var, float y_var, float z_var){
	opName = name;
	x = x_var;
	y = y_var;
	z = z_var;
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

Translate::Translate(char* name, float x, float y, float z, float t) : Oper(name,x,y,z){
	time = t;
}

void catmullRomPoint(float t, float *p, std::vector<Vertex*> points){
    int size = points.size();

    float rt = t*size;
    int i = floor(rt);
    rt = rt - i;

    int index[4];
    index[0] = (i  + size - 1) % size;
    index[1] = (index[0]  + 1) % size;
    index[2] = (index[1]  + 1) % size;
    index[3] = (index[2]  + 1) % size;

    catmullCalculate(rt,index,p,points);
}

void catmullCalculate(float t, int* index, float* p, std::vector<Vertex*> points){
    float aux[4];
    float t_2 = t*t;
    float t_3 = t*t*t;

    float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
                                  { 1.0f, -2.5f, 2.0f, -0.5f },
                                  { -0.5f, 0.0f, 0.5f, 0.0f },
                                  { 0.0f, 1.0f, 0.0f, 0.0f }};

    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;

    aux[0] = t_3*m[0][0] + t_2*m[1][0] + t*m[2][0] + m[3][0];
    aux[1] = t_3*m[0][1] + t_2*m[1][1] + t*m[2][1] + m[3][1];
    aux[2] = t_3*m[0][2] + t_2*m[1][2] + t*m[2][2] + m[3][2];
    aux[3] = t_3*m[0][3] + t_2*m[1][3] + t*m[2][3] + m[3][3];


    int i_0 = index[0];
    int i_1 = index[1];
    int i_2 = index[2];
    int i_3 = index[3];
    Vertex* v0 = points[i_0];
    Vertex* v1 = points[i_1];
    Vertex* v2 = points[i_2];
    Vertex* v3 = points[i_3];

    p[0] = aux[0] * v0->getX() + aux[1] * v1->getX() + aux[2] * v2->getX() + aux[3] * v3->getX();
    p[1] = aux[0] * v0->getY() + aux[1] * v1->getY() + aux[2] * v2->getY() + aux[3] * v3->getY();
    p[2] = aux[0] * v0->getZ() + aux[1] * v1->getZ() + aux[2] * v2->getZ() + aux[3] * v3->getZ();
}

void Translate::generateCurve(){
    float p[3];

    if(catmullPoints.size() != 0) {
        for (float gt = 0; gt < 1; gt += 0.01) {
            catmullRomPoint(gt, p, catmullPoints);
            curvePoints.push_back(new Vertex(p[0], p[1], p[2]));
        }
    }
    else return;
}

int Translate::getCurveSize(){
    return catmullPoints.size();
}

void Translate::drawCurve() {
    if (curvePoints.size() != 0) {

        glBegin(GL_LINE_LOOP);

        for (Vertex* v: curvePoints) {
            glVertex3f(v->getX(), v->getY(), v->getZ());
        }

        glEnd();
    }
}

void Translate::addPoint(Vertex* v){
    catmullPoints.push_back(v);
}

void Translate::draw() {
    float coord[3] = {getX(),getY(),getZ()};

    if(time>0){
        float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int) (time * 1000);
        float rt = elapsed/(time*1000);

        catmullRomPoint(rt,coord,catmullPoints);
    }

    glTranslatef(coord[0],coord[1],coord[2]);
}

Rotate::Rotate(char* name, float x, float y, float z,float a, float t) : Oper(name,x,y,z){
    angle = a;
    time = t;
}

void Rotate::draw(){
    float currAngle = angle;

    if(time > 0){
        float elapsed = glutGet(GLUT_ELAPSED_TIME) % (int) (time * 1000);
        currAngle = (elapsed *360) / (time * 1000);
    }

    glRotatef(currAngle,getX(),getY(),getZ());
}


//Group
Group::Group(){
}

Group::Group(vector<Oper*> op, vector<Model*> m, vector<Group*> c){
    operations = op;
    models = m;
    children = c;
}

Light::Light(float *p){
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
    pos[3] = p[3];
}

void Light::renderLight() {
    GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diff[4] = {1.0, 1.0, 1.0, 0};
    GLfloat spec[4] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
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

vector<Light*> Group::getLight(){
    return lights;
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

void Group::pushLight(Light* l){
    lights.push_back(l);
}

Group::~Group(void) {
}

void Model::pushMaterials(float d[4], float s[4], float e[4], float a[4], float shine){
    ambient[0] = a[0];
    ambient[1] = a[1];
    ambient[2] = a[2];
    ambient[3] = a[3];

    diffuse[0] = d[0];
    diffuse[1] = d[1];
    diffuse[2] = d[2];
    diffuse[3] = d[3];

    specular[0] = s[0];
    specular[1] = s[1];
    specular[2] = s[2];
    specular[3] = s[3];

    emission[0] = e[0];
    emission[1] = e[1];
    emission[2] = e[2];
    emission[3] = e[3];

    shininess = shine;
}

void Model::setUpVBO(vector<Vertex*> vert, vector<Vertex*> norm, vector<Vertex*> tex, vector<float> ind){
    buff_size[0] = ind.size();
    buff_size[1] = ind.size();
    buff_size[2] = ind.size();

    float* vertex_array = (float*) malloc(sizeof(float)  * 3 * ind.size());
    float* normal_array = (float*) malloc(sizeof(float)  * 3 * ind.size());
    float* texture_array = (float*) malloc(sizeof(float) * 2 * ind.size());


    int index=0;
    int cont=0;
    for(float n : ind){
       

        vertex_array[cont] = vert[n]->getX();
        vertex_array[cont+1] = vert[n]->getY();
        vertex_array[cont+2] = vert[n]->getZ();
        /*
        float x1, x2, x3;
        x1 = normal_array[cont];
        x2 = vertex_array[cont + 1];
        x3 = vertex_array[cont + 2]; 
        cout << x1 << ',' << x2 << ',' << x3 << '\n'; */

        normal_array[cont] = norm[n]->getX();
        normal_array[cont+1] = norm[n]->getY();
        normal_array[cont+2] = norm[n]->getZ();
        cont+=3;

        texture_array[index] = tex[n]->getX();
        texture_array[index + 1] = tex[n]->getY();
        index += 2;
        
    }

    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[0] * 3, vertex_array, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[1] * 3, normal_array, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buff_size[2] * 2, texture_array, GL_STATIC_DRAW);

    free(vertex_array);
    free(normal_array);
    free(texture_array);
}

void Model::setUpTexture(string t_file){
    unsigned int t,tw,th;
    unsigned char *texData;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) t_file.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::renderModel(){
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if(buff_size[1]){
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if(buff_size[2]){
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glDrawArrays(GL_TRIANGLES, 0, buff_size[0] * 3);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Model::~Model(void) {
}