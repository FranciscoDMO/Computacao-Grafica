#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <string>

class Vertex{
    private:
        float x;
        float y;
        float z;

    public:
        Vertex();
        Vertex(float xx, float yy, float zz);
        float getX();
        float getY();
        float getZ();
};

#endif