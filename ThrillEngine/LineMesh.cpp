#include "LineMesh.h"

LineMesh::LineMesh()
{
    VBO_positions = 0;
    VBO_normals = 0;
    EBO = 0;
    index_size = 0;
}

void LineMesh::Init()
{
    glGenBuffers(1, &VBO_positions);
}

void LineMesh::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * positions.size() * 3), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void LineMesh::UnBind()
{
    glDisableVertexAttribArray(0);
}

void LineMesh::Draw(Shader shader, Material* custom_material)
{
    Bind();
    glDrawArrays(GL_LINES,0, static_cast<GLsizei>(positions.size()));
    UnBind();
}

LineMesh::~LineMesh()
{
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
}
