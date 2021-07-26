#include <cstdio>
#include <cstring>
#include <vector>
#include <math.h>
#include "models.h"
#define M_PI 3.14
void criarPlano(float size, char *fname){
    FILE * f = fopen(fname,"w");
    float x, z;
    x = z = size / 2;

    fprintf(f, "%f %f %f\n", x, 0.0, z);
    fprintf(f, "%f %f %f\n", x, 0.0, -z);
    fprintf(f, "%f %f %f\n", -x, 0.0, -z);

    fprintf(f, "%f %f %f\n", x, 0.0, z);
    fprintf(f, "%f %f %f\n", -x, 0.0, -z);
    fprintf(f, "%f %f %f\n", -x, 0.0, z);

    fclose(f);
}

void criarCone(float radius, float height, int slices, int stacks, char *fname){
    float hdiff = height / stacks;
    float rdiff = radius / stacks;
    float step_angle = (2 * M_PI) / slices;
    float angle = 0.0f;
    
    FILE* f = fopen(fname, "w");
    
    // base
    int slice;
    for(slice = 0; slice < slices; slice++) {
        float x1 = radius * sin(angle);
        float z1 = radius * cos(angle);
        
        float x2 = radius * sin(angle + step_angle);
        float z2 = radius * cos(angle + step_angle);
        
        fprintf(f, "%f %f %f\n", x1, 0.f, z1);
        fprintf(f, "%f %f %f\n", 0.f, 0.f, 0.f);
        fprintf(f, "%f %f %f\n", x2, 0.f, z2);
        
        angle += step_angle;
    }
    
    // face lateral
    height = 0;
    angle = 0.0f;
    int stack;
    for(stack = 0; stack < stacks; stack++) {
        for(slice = 0; slice < slices; slice++) {
            float px1 = radius * sin(angle);
            float py1 = height;
            float pz1 = radius * cos(angle);
            
            float px2 = radius * sin(angle + step_angle);
            float py2 = height;
            float pz2 = radius * cos(angle + step_angle);
            
            float px3 = (radius - rdiff) * sin(angle + step_angle);
            float py3 = height + hdiff;
            float pz3 = (radius - rdiff) * cos(angle + step_angle);
            
            float px4 = (radius - rdiff) * sin(angle);
            float py4 = height + hdiff;
            float pz4 = (radius - rdiff) * cos(angle);
            
            fprintf(f, "%f %f %f\n", px1, py1, pz1);
            fprintf(f, "%f %f %f\n", px2, py2, pz2);
            fprintf(f, "%f %f %f\n", px4, py4, pz4);
            fprintf(f, "%f %f %f\n", px3, py3, pz3);
            fprintf(f, "%f %f %f\n", px4, py4, pz4);
            fprintf(f, "%f %f %f\n", px2, py2, pz2);
            
            angle -= step_angle;
        }
        
        height += hdiff;
        radius -= rdiff;
    }
    
    fclose(f);

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
    
    FILE* f = fopen(fname, "w");
    
    // planos z
    y_ = 0.0f;
    for(line = 0; line < divs; line++) {
        x_ = -xc;
        
        for(col = 0; col < divs; col++) {
            // frente (z = zc) - mão direita
            fprintf(f, "%f %f %f\n", x_, y_, zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_, zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_ + step_y, zc);
            
            fprintf(f, "%f %f %f\n", x_, y_, zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_ + step_y, zc);
            fprintf(f, "%f %f %f\n", x_, y_ + step_y, zc);
            
            // trás (z = -zc) - mão esquerda
            fprintf(f, "%f %f %f\n", x_, y_, -zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_ + step_y, -zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_, -zc);
            
            fprintf(f, "%f %f %f\n", x_, y_, -zc);
            fprintf(f, "%f %f %f\n", x_, y_ + step_y, -zc);
            fprintf(f, "%f %f %f\n", x_ + step_x, y_ + step_y, -zc);
            
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
            fprintf(f, "%f %f %f\n", xc, y_, z_);
            fprintf(f, "%f %f %f\n", xc, y_, z_ - step_z);
            fprintf(f, "%f %f %f\n", xc, y_ + step_y, z_ - step_z);
            
            fprintf(f, "%f %f %f\n", xc, y_, z_);
            fprintf(f, "%f %f %f\n", xc, y_ + step_y, z_ - step_z);
            fprintf(f, "%f %f %f\n", xc, y_ + step_y, z_);
            
            // esquerda (x = -xc) - mão esquerda
            fprintf(f, "%f %f %f\n", -xc, y_, z_);
            fprintf(f, "%f %f %f\n", -xc, y_ + step_y, z_ - step_z);
            fprintf(f, "%f %f %f\n", -xc, y_, z_ - step_z);
            
            fprintf(f, "%f %f %f\n", -xc, y_, z_);
            fprintf(f, "%f %f %f\n", -xc, y_ + step_y, z_);
            fprintf(f, "%f %f %f\n", -xc, y_ + step_y, z_ - step_z);
            
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
            fprintf(f, "%f %f %f\n", x_, y, z_);
            fprintf(f, "%f %f %f\n", x_, y, z_ + step_z);
            fprintf(f, "%f %f %f\n", x_ + step_x, y, z_);
            fprintf(f, "%f %f %f\n", x_ + step_x, y, z_);
            fprintf(f, "%f %f %f\n", x_, y, z_ + step_z);
            fprintf(f, "%f %f %f\n", x_ + step_x, y, z_ + step_z);
            
            // baixo (y = 0) - mão esquerda
            fprintf(f, "%f %f %f\n", x_, 0.f, z_);
            fprintf(f, "%f %f %f\n", x_ + step_x, 0.f, z_);
            fprintf(f, "%f %f %f\n", x_, 0.f, z_ + step_z);
            fprintf(f, "%f %f %f\n", x_ + step_x, 0.f, z_);
            fprintf(f, "%f %f %f\n", x_ + step_x, 0.f, z_ + step_z);
            fprintf(f, "%f %f %f\n", x_, 0.f, z_ + step_z);
            
            x_ += step_x;
        }
        
        z_ += step_z;
    }
    
    fclose(f);
}

void criarEsfera(float radius, int slices, int stacks, char *fname){
    float alpha_step = (2 * M_PI) / slices;
    float alpha = 0.0f;
    float beta_step  = M_PI / stacks;
    float beta = -M_PI / 2;
    
    FILE* f = fopen(fname, "w");
    
    // Declaracao das variaveis dos pontos
    float px1, py1, pz1;
    float px2, py2, pz2;
    float px3, py3, pz3;
    float px4, py4, pz4;
    
    // Base inferior
    int slice;
    for(slice = 0; slice < slices; slice++) {
        px1 = radius * cos(beta + beta_step) * sin(alpha);
        py1 = radius * sin(beta + beta_step);
        pz1 = radius * cos(beta + beta_step) * cos(alpha);
        
        px2 = 0.0f;
        py2 = radius * sin(beta);
        pz2 = 0.0f;
        
        px3 = radius * cos(beta + beta_step) * sin(alpha + alpha_step);
        py3 = radius * sin(beta + beta_step);
        pz3 = radius * cos(beta + beta_step) * cos(alpha + alpha_step);
        
        fprintf(f, "%f %f %f\n", px1, py1, pz1);
        fprintf(f, "%f %f %f\n", px2, py2, pz2);
        fprintf(f, "%f %f %f\n", px3, py3, pz3);
        
        alpha += alpha_step;
    }
    
    beta += beta_step;
    
    // face lateral
    int stack;
    for(stack = 1; stack < stacks - 1; stack++) {
        for(slice = 0, alpha = 0.0f; slice < slices; slice++) {
            px1 = radius * cos(beta) * sin(alpha);
            py1 = radius * sin(beta);
            pz1 = radius * cos(beta) * cos(alpha);
            
            px2 = radius * cos(beta) * sin(alpha + alpha_step);
            py2 = radius * sin(beta);
            pz2 = radius * cos(beta) * cos(alpha + alpha_step);
            
            px3 = radius * cos(beta + beta_step) * sin(alpha + alpha_step);
            py3 = radius * sin(beta + beta_step);
            pz3 = radius * cos(beta + beta_step) * cos(alpha + alpha_step);
            
            px4 = radius * cos(beta + beta_step) * sin(alpha);
            py4 = radius * sin(beta + beta_step);
            pz4 = radius * cos(beta + beta_step) * cos(alpha);
            
            
            fprintf(f, "%f %f %f\n", px1, py1, pz1);
            fprintf(f, "%f %f %f\n", px2, py2, pz2);
            fprintf(f, "%f %f %f\n", px4, py4, pz4);
            
            fprintf(f, "%f %f %f\n", px3, py3, pz3);
            fprintf(f, "%f %f %f\n", px4, py4, pz4);
            fprintf(f, "%f %f %f\n", px2, py2, pz2);
            
            alpha += alpha_step;
        }
        
        beta += beta_step;
    }
    
    // Base superior
    for(slice = 0; slice < slices; slice++) {
        px1 = radius * cos(beta) * sin(alpha);
        py1 = radius * sin(beta);
        pz1 = radius * cos(beta) * cos(alpha);
        
        px2 = 0.0f;
        py2 = radius * sin(beta + beta_step);
        pz2 = 0.0f;
        
        px3 = radius * cos(beta) * sin(alpha + alpha_step);
        py3 = radius * sin(beta);
        pz3 = radius * cos(beta) * cos(alpha + alpha_step);
        
        fprintf(f, "%f %f %f\n", px1, py1, pz1);
        fprintf(f, "%f %f %f\n", px3, py3, pz3);
        fprintf(f, "%f %f %f\n", px2, py2, pz2);
        
        alpha += alpha_step;
    }
    
    fclose(f);

   
}
