#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "models.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 4) {
        return printf("Not enough parameters!");
    }

    if (argc > 7) {
        return printf("Too many arguments!");
    }

    char* shape = argv[1];
    char* file = argv[argc - 1];


    if (strcmp(shape, "plane") == 0) {
        if (argc != 4) {
            return printf("Number of parameters wrong!");
        }
        criarPlano(atof(argv[2]), file);
    }

    else if (strcmp(shape, "sphere") == 0) {
        if (argc != 6) {
            printf("Number or parameters wrong!");
        }
        criarEsfera(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), file);
    }

    else if (strcmp(shape, "cone") == 0) {
        if (argc != 7) {
            return printf("Number or parameters wrong!");
        }
        criarCone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), file);
    }

    else if (strcmp(shape, "box") == 0) {
        if (argc == 6) {
            criarCaixa(atof(argv[2]), atof(argv[3]), atof(argv[4]), 1, file);
        }

        else if (argc == 7) {
            criarCaixa(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), file);
        }

        else {
            return printf("Number or parameters wrong!");
        }


    }
    //  File , tesselation, endfile
    else if (strcmp(shape, "bezier") == 0) {
        
        if (argc == 5) {
            
            bezier(argv[2], atoi(argv[3]), file);

        }
        else {
            return printf("Number or parameters wrong!");
        }

        return 0;
    }
}

