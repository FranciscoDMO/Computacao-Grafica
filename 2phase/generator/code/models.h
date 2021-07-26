#ifndef _MODELS_H
#define _MODELS_H

#include <vector>
#include <math.h>

void criarPlano(float size, char *fname);
void criarCaixa(float x, float y, float z, int divs ,char *fname);
void criarCone(float radius, float height, int slices, int stacks, char *fname);
void criarEsfera(float radius, int slices, int stacks, char *fname);

#endif