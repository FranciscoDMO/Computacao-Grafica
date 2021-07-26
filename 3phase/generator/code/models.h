#ifndef _MODELS_H
#define _MODELS_H

#include <cstdio>
#include <cstring>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <iostream>
#include "vertices.h"
using namespace std;

void criarPlano(float size, char *fname);
void imprime(std::vector<Vertice> vertices, std::vector<int> verticesF, std::vector<Vertice> norm, std::vector<Vertice> text, char* fname);
void criarCaixa(float x, float y, float z, int divs ,char *fname);
void criarCone(float radius, float height, int slices, int stacks, char *fname);
void criarEsfera(float radius, int slices, int stacks, char *fname); 
void bezier(string fnameI, int tesselation, char* fname);
void getBezier(int tesselation, std::vector<int>  indice, std::vector<Vertice>  vert, int psize);
Vertice* bezierCalculate(float u, float v, int n_patch, std::vector<Vertice>  vert, std::vector<int>  indices);
Vertice* vertexCalculate(float t, float* p1, float* p2, float* p3, float* p4);
void imprimeBezier(char* fname);
#endif