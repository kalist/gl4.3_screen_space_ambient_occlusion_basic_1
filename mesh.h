/****************************************************************************
**
** Piotr Sydow, 2014
** Contact: piotr.sydow.gm@gmail.com
**
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once
#ifndef MESH_H
#define MESH_H
#include "GL.h"
#include "mtools/vmath.h"
#include "tools/glh.h"
#include <vector>
#include <map>
#include <cstdio>
#include <stdio.h>
#include <QDebug>
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"

enum AXIS {
    X = 0,
    Y = 1,
    Z = 2,
    W = 3
};

typedef struct Vertex{
 public:
    Vertex() {}
    Vertex(const vmath::vec3& position,
           const vmath::vec2& texture,
           const vmath::vec3& normal,
           const vmath::vec3& binormal,
           const vmath::vec3& tangent
    )
    {
        m_position = position;
        m_texture = texture;
        m_normal = normal;
        m_binormal = binormal;
        m_tangent = tangent;

    }
    vmath::vec3 m_position;
    vmath::vec2 m_texture;
    vmath::vec3 m_normal;
    vmath::vec3 m_binormal;
    vmath::vec3 m_tangent;
}Vertex;



typedef struct MeshEntry {
public:
    MeshEntry();
    ~MeshEntry();

    void Init(const std::vector<Vertex>& vertices,
              const std::vector<unsigned int>& indices
    );

    void Bind();
    void Unbind();
    void Draw();

    GLuint vbo;
    GLuint ibo;
    GLuint vao;
    unsigned int numIndices;
    unsigned int materialIndex;
}MeshEntry;


class Mesh{

public:
     Mesh(char* fileName, bool load);
    ~Mesh();

    int Load();
    int Unload();
    int Allocate();
    int Deallocate();
    void Draw();

private:
    char* m_fileName;
    std::vector<MeshEntry> m_entries;
};


#endif // MESH_H
