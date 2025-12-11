#pragma once
#include <glad/glad.h>
#include <vector>

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    void load_cube(); // For this phase, just hardcoded cube
    void draw();

private:
    unsigned int VAO, VBO;
    int vertex_count;
};
