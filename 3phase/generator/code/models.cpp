#include <stdlib.h>
#include "models.h"
#define M_PI 3.14
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
void criarPlano(float size, char *fname){
    float x, z;
    x = z = size / 2;
    std::vector<Vertice> vertices; // vetor com todos os pontos   
    std::vector<Vertice> normais; //vetor com as normais 
    std::vector<Vertice> textura; //vetor com pontos das texturas 
    std::vector<int> indices;

    vertices.push_back(Vertice (x, 0.0, z));
    vertices.push_back(Vertice(-x, 0.0, -z));
    vertices.push_back(Vertice(-x, 0.0, z));
    vertices.push_back(Vertice(x, 0.0, -z));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(3);

    normais.push_back(Vertice(0.0f, 1.0f, 0.0f));
    normais.push_back(Vertice(0.0f, 1.0f, 0.0f));
    normais.push_back(Vertice(0.0f, 1.0f, 0.0f));
    normais.push_back(Vertice(0.0f, 1.0f, 0.0f));

    textura.push_back(Vertice(1.0f, 1.0f, 0.0f));
    textura.push_back(Vertice(0.0f, 0.0f, 0.0f));
    textura.push_back(Vertice(0.0f, 1.0f, 0.0f));
    textura.push_back(Vertice(1.0f, 0.0f, 0.0f));

    imprime(vertices, indices, normais, textura, fname);

}

void criarEsfera(float radius, int slices, int stacks, char* fname) {

    std::vector<Vertice> vertices; // vetor com todos os pontos   
    std::vector<Vertice> normais; //vetor com as normais 
    std::vector<Vertice> textura; //vetor com pontos das texturas 

    for (int stack = 0; stack <= stacks; stack++) {

        float alphaI = static_cast<float>((M_PI * 2) / slices);
        float betaI = static_cast<float>((M_PI) / stacks);
        float beta = stack * betaI; // current vertical angle

        for (int slice = 0; slice <= slices; slice++) {

            float alpha = slice * alphaI;
                       
            float x = radius * sin(beta) * cos(alpha);
            float y = radius * cos(beta);
            float z = radius * sin(beta) * sin(alpha);

            vertices.push_back(Vertice(x, y, z));

            x = x / radius;
            y = y / radius;
            z = z / radius;
            normais.push_back(Vertice(x, y, z));;


            x = (slices - (float)slice) / slices;
            y = (stacks - (float)stack) / stacks;
            textura.push_back(Vertice(x, y, 0));;

        }
    }

    std::vector<int> indices;
    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (slices + 1);     // beginning of current stack
        k2 = k1 + slices + 1;      // beginning of next stack

        for (int j = 0; j < slices; ++j, ++k1, ++k2)
        {
            
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    imprime(vertices, indices, normais, textura, fname);

}

void criarCone(float radius, float height, int slices, int stacks, char *fname){
    float hdiff = height / stacks;
    float rdiff = radius / stacks;
    float step_angle = (2 * M_PI) / slices;
    float angle = 0.0f;
    float heightc = height / 2;
    
    std::vector<Vertice> vertices;
    std::vector<int> indice; //vetor sem pontos repetidos
    std::vector<Vertice> normais; //vetor com as normais 
    std::vector<Vertice> textura; //vetor com pontos das texturas 
     
    
    // base 
    vertices.push_back(Vertice(0.f, -heightc, 0.f));
    normais.push_back(Vertice(0.f, -heightc, 0.f));
    textura.push_back(Vertice(0.f , 1 , 0.0f));
    
    for(int slice = 1; slice <= slices; slice++) {

        float x1 = radius * sin(angle + step_angle);
        float y1 = -heightc;
        float z1 = radius * cos(angle + step_angle);
        
        
        vertices.push_back(Vertice(x1, y1, z1));
        normais.push_back(Vertice(0.f, y1, 0.f));
        
        
        float b = slice / slices;   
        textura.push_back(Vertice(b, 0, 0.0f));
        
        

       
        
        angle += step_angle;
    }
    for (int slice = 0; slice < slices; slice++) {
        indice.push_back(0);
        indice.push_back(slice*2+2);
        indice.push_back(slice*2+1);
           
    }
      
    // face lateral
    angle = 0.0f;
    for (int slice = 0; slice < slice; slice++) {
        for (int stack = 0; stack < stacks + 1; stack++) {


            float div = stack / stacks;
            float alpha = (1.0 - (div)) * radius;

            float px = alpha * cos(angle + step_angle);
            float py = stack * div - heightc;
            float pz = alpha * sin(angle + step_angle);

            float nx = cos(angle + step_angle);
            float ny = sin(M_PI - atan(height / radius));
            float nz = cos(angle + step_angle);

            normais.push_back(Vertice(nx, ny, nz));

            float tx = slice / slices;
            float ty = stack / stacks;

            textura.push_back(Vertice(tx, ty, 0));

            angle += step_angle;
        }

        for (int slice = 0; slice < slices; slice++) {
            for (int stack = 0; stack < stacks; stacks++) {
                indice.push_back((slices + 1) * 2 + slice * (slices + 1) + stack);
                indice.push_back((slices + 1) * 2 + slice * (slices + 1) + stack + 1);
                indice.push_back((slices + 1) * 2 + (slice + 1) * (slices + 1) + stack);
                
                indice.push_back((slices + 1) * 2 + (slice + 1) * (slices + 1) + stack);
                indice.push_back((slices + 1) * 2 + slice * (slices + 1) + stack + 1);
                indice.push_back((slices + 1) * 2 + (slice + 1) * (slices + 1) + stack + 1);

            }
        }
    }
    
    imprime(vertices, indice, normais, textura, fname);

}


void criarCaixa(float x, float y, float z, int divs, char *fname){
    float step_x = x / divs;
    float step_y = y / divs;
    float step_z = z / divs;
    
    // limites para centrar a caixa na origem
    float xc = x / 2;
    float zc = z / 2;
    
    float x_, y_, z_;
    int line, col;
    std::vector<Vertice> vertices;
    std::vector<Vertice> verticesF;
    std::vector<Vertice> normais; //vetor com as normais 
    std::vector<Vertice> textura; //vetor com pontos das texturas 
    FILE* f = fopen(fname, "w");

    
    // planos z
    y_ = 0.0f;
    for(line = 0; line < divs; line++) {
        x_ = -xc;
        
        for(col = 0; col < divs; col++) {
            // frente (z = zc) - mão direita
            vertices.push_back(Vertice(x_, y_, zc));
            vertices.push_back(Vertice(x_ + step_x, y_, zc));
            vertices.push_back(Vertice(x_ + step_x, y_ + step_y, zc));
            normais.push_back(Vertice(0.f, 0.f, 1));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));

            vertices.push_back(Vertice(x_, y_, zc));
            vertices.push_back(Vertice(x_ + step_x, y_ + step_y, zc));
            vertices.push_back(Vertice(x_, y_ + step_y, zc));
            normais.push_back(Vertice(0.f,0.f,1));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));

            // trás (z = -zc) - mão esquerda
            vertices.push_back(Vertice(x_, y_, -zc));
            vertices.push_back(Vertice(x_ + step_x, y_ + step_y, -zc));
            vertices.push_back(Vertice(x_ + step_x, y_, -zc));
            normais.push_back(Vertice(0.f, 0.f, -1));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));

            vertices.push_back(Vertice(x_, y_, -zc));
            vertices.push_back(Vertice(x_, y_ + step_y, -zc));
            vertices.push_back(Vertice(x_ + step_x, y_ + step_y, -zc));
            normais.push_back(Vertice(0.f, 0.f, -1));
            textura.push_back(Vertice( col / divs, line / divs, 0.f));
            x_ += step_x;
        }
        
        y_ += step_y;
    }
    
    // planos x
    y_ = 0.0f;
    for(line = 0; line < divs; line++) {
        z_ = zc;
        
        for(col = 0; col < divs; col++) {
            // direita (x = xc) - mão direita
            vertices.push_back(Vertice(xc, y_, z_));
            vertices.push_back(Vertice(xc, y_, z_ - step_z));
            vertices.push_back(Vertice(xc, y_ + step_y, z_ - step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(1, 0.f, 0.f));
            
            vertices.push_back(Vertice(xc, y_, z_));
            vertices.push_back(Vertice(xc, y_ + step_y, z_ - step_z));
            vertices.push_back(Vertice(xc, y_ + step_y, z_));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(1, 0.f, 0.f));
            // esquerda (x = -xc) - mão esquerda
            vertices.push_back(Vertice(-xc, y_, z_));
            vertices.push_back(Vertice(-xc, y_ + step_y, z_ - step_z));
            vertices.push_back(Vertice(-xc, y_, z_ - step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(-1, 0.f, 0.f));
            
            vertices.push_back(Vertice(-xc, y_, z_));
            vertices.push_back(Vertice(-xc, y_ + step_y, z_));
            vertices.push_back(Vertice(-xc, y_ + step_y, z_ - step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(-1, 0.f, 0.f));
            z_ -= step_z;
        }
        
        y_ += step_y;
    }
    
    // planos y
    z_ = -zc;
    for(line = 0; line < divs; line++) {
        x_ = -xc;
        for(col = 0; col < divs; col++) {
            // cima (y = y) - mão direita
            vertices.push_back(Vertice(x_, y, z_));
            vertices.push_back(Vertice(x_, y, z_ + step_z));
            vertices.push_back(Vertice(x_ + step_x, y, z_));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(0.f, 1, 0.f));

            vertices.push_back(Vertice(x_ + step_x, y, z_));
            vertices.push_back(Vertice(x_, y, z_ + step_z));
            vertices.push_back(Vertice(x_ + step_x, y, z_ + step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(0.f, 1, 0.f));

            // baixo (y = 0) - mão esquerda
            vertices.push_back(Vertice(x_, 0.f, z_));
            vertices.push_back(Vertice(x_ + step_x, 0.f, z_));
            vertices.push_back(Vertice(x_, 0.f, z_ + step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(0.f, -1, 0.f));

            vertices.push_back(Vertice(x_ + step_x, 0.f, z_));
            vertices.push_back(Vertice(x_ + step_x, 0.f, z_ + step_z));
            vertices.push_back(Vertice(x_, 0.f, z_ + step_z));
            textura.push_back(Vertice(col / divs, line / divs, 0.f));
            normais.push_back(Vertice(0.f, -1, 0.f));

            x_ += step_x;
        }
        
        z_ += step_z;
    }

    for (int nvert = 0; nvert < vertices.size(); nvert++) {
        int found = 0;
        for (int nvertf = 0; found == 0 && nvertf < verticesF.size(); nvertf++) {
            if ((vertices[nvert].getX() == vertices[nvertf].getX()) && (vertices[nvert].getY() == vertices[nvertf].getY()) && (vertices[nvert].getZ() == vertices[nvertf].getZ())) {
                found = 1;

            }
        }
        if (found == 0) {
            float x1 = vertices[nvert].getX();
            float y1 = vertices[nvert].getY();
            float z1 = vertices[nvert].getZ();
            verticesF.push_back(Vertice(x1, y1, z1));
            fprintf(f, "%f %f %f \n", x1, y1, z1);
        }

    }

    for (int nindex = 0; nindex < vertices.size(); nindex++) {
        for (int i = 0; i < verticesF.size(); i++) {
            if ((vertices[nindex].getX() == verticesF[i].getX()) && (vertices[nindex].getY() == verticesF[i].getY()) && (vertices[nindex].getZ() == verticesF[i].getZ())) {
                fprintf(f, "%i \n", i);
                break;
            }
        }
    }


    for (int i = 0; i < normais.size(); i++) {
        float x1 = normais[i].getX();
        float y1 = normais[i].getY();
        float z1 = normais[i].getZ();
        fprintf(f, "%f %f %f \n", x1, y1, z1);

    }



    for (int i = 0; i < textura.size(); i++) {
        float x1 = textura[i].getX();
        float y1 = textura[i].getY();
        fprintf(f, "%f %f \n", x1, y1);

    }
    
    fclose(f);
}

vector<Vertice*> vertF;

void bezier (string fnameI, int tesselation, char* fname) {

   string line;
    ifstream file(fnameI); // object from which characters are extracted.
    getline(file ,line);

    int psize = stoi(line);
    std::vector<int> indice;
    
    
    for (int i = 0; i != psize; i++) {
        getline(file, line);
        
        for (int j = 0; j < 16; j++) {
            int pos = line.find(",");
            string token = line.substr(0, pos);
            int bla = atoi(token.c_str());
            
            indice.push_back(bla);
            line.erase(0, pos + 1);
            
        }
        
        
    }

    getline(file, line);

    vector<Vertice> vert; // contem os vertices 

    while (getline(file, line)) {
        float x, y, z =0;
        
        for (int j = 0; j < 3; j++) {
            int pos = line.find(",");
            string num = line.substr(0, pos);
            if(j==0 || j==1){
                if (j == 0) { x = stof(num); 
                }
                else if (j == 1) {
                    y = stof(num); 
                }
                line.erase(0, pos + 1);
               
            }
            
            else if (j == 2) {
                int pos = line.find('\n');
                string num = line.substr(0, pos); 
                z = stof(num); 
            }

    
        }
            
            vert.push_back(Vertice(x, y, z));
            
        }
     
        for (int i = 0; i != psize; i++) {
            getBezier(tesselation, indice, vert, i);
        }

        imprimeBezier( fname);
       
}

void getBezier(int tesselation , std::vector<int>  indice, std::vector<Vertice>  vert, int psize) {
    
    float inc = 1.0 / tesselation, u, v, u2, v2;
    
    
    for (int i = 0; i < tesselation; i++) {
        
        for (int j = 0; j < tesselation; j++) {
            
            u = i * inc;
            v = j * inc;
            u2 = (i + 1) * inc;
            v2 = (j + 1) * inc;
            
            
            Vertice* p1 = bezierCalculate(u, v, psize, vert, indice);
            Vertice* p2 = bezierCalculate(u, v2, psize, vert, indice);
            Vertice* p3 = bezierCalculate(u2, v, psize, vert, indice);
            Vertice* p4 = bezierCalculate(u2, v2, psize, vert, indice);


            vertF.push_back(p1);
            vertF.push_back(p3);
            vertF.push_back(p4);

            vertF.push_back(p1);
            vertF.push_back(p4);
            vertF.push_back(p2);
            
           
            
        }
    }

}

void imprimeBezier(char* fname) {
    FILE* f = fopen(fname, "w");

    for (int nVert = 0; nVert < vertF.size(); nVert++) {
        float x1 = vertF[nVert]->getX();
        float x2 = vertF[nVert]->getY();
        float x3 = vertF[nVert]->getZ();


        fprintf(f, "%f %f %f \n", x1, x2, x3);

    }
    fclose(f);
}

Vertice* bezierCalculate(float u, float v, int n_patch, std::vector<Vertice>  vert , std::vector<int>  indice) {
    float bz_p[4][3], res[4][3];
    int j = 0, k = 0;
    int n_patch1 = (16 * n_patch);

    for (int i = 0; i < 16; i++) {
        
        int j1 = indice[n_patch1 + i];
       
 
        bz_p[j][0] = vert.at(j1).getX();
        bz_p[j][1] = vert.at(j1).getY();
        bz_p[j][2] = vert.at(j1).getZ();
        j++;

        if (j % 4 == 0) {
            Vertice* v = vertexCalculate(u, bz_p[0], bz_p[1], bz_p[2], bz_p[3]);

            res[k][0] = v->getX();
            res[k][1] = v->getY();
            res[k][2] = v->getZ();

            k++;
            j = 0;
        }
    }

    return vertexCalculate(v, res[0], res[1], res[2], res[3]);
}

Vertice* vertexCalculate(float t, float* p1, float* p2, float* p3, float* p4) {
    float coords[3];
    float ite = 1.0 - t;

    float bz1 = ite * ite * ite;
    float bz2 = 3 * t * ite * ite;
    float bz3 = 3 * t * t * ite;
    float bz4 = t * t * t;
    
        
    coords[0] = bz1 * p1[0] + bz2 * p2[0] + bz3 * p3[0] + bz4 * p4[0];
    coords[1] = bz1 * p1[1] + bz2 * p2[1] + bz3 * p3[1] + bz4 * p4[1];
    coords[2] = bz1 * p1[2] + bz2 * p2[2] + bz3 * p3[2] + bz4 * p4[2];
   
    return ( new Vertice (coords[0], coords[1], coords[2]));
}


void imprime(std::vector<Vertice> vertices, std::vector<int> verticesF, std::vector<Vertice> norm, std::vector<Vertice> text, char* fname) {
    FILE* f = fopen(fname, "w");

    for (int i = 0; i < vertices.size(); i++) {
        float x1 = vertices[i].getX();
        float y1 = vertices[i].getY();
        float z1 = vertices[i].getZ();

        fprintf(f, "V %f %f %f \n", x1, y1, z1);
    }

    for (int i = 0; i < norm.size(); i++) {
        float x1 = norm[i].getX();
        float y1 = norm[i].getY();
        float z1 = norm[i].getZ();
        fprintf(f, "N %f %f %f \n", x1, y1, z1);

    }

    float z = 0;
    for (int i = 0; i < text.size(); i++) {
        float x1 = text[i].getX();
        float y1 = text[i].getY();
        fprintf(f, "T %f %f %f\n", x1, y1, z);
    }

    for (int i = 0; i < verticesF.size(); i++) {
        float x1 = verticesF[i];
        
        fprintf(f, "I %f \n", x1);
    }

    fclose(f);

}
