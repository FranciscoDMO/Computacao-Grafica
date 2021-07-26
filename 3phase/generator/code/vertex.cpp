#include "vertex.h"

// Vertex Construtor vazio
Vertex::Vertex(){
    x = 0;
    y = 0;
    z = 0;
}

// Vertex Construtor por paraâmetros
Vertex::Vertex(float xx, float yy, float zz){
    x = xx;
    y = yy;
    z = zz;
}

float Vertex::getX(){
    return x;
}

float Vertex::getY(){
    return y;
}

float Vertex::getZ(){
    return z;
}