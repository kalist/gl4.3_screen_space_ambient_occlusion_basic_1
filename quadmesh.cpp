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

#include "GL/glew.h"
#include "quadmesh.h"

QuadMesh::QuadMesh(bool load, bool allocate)
{
    loaded = false;
    allocated = false;

    vao = INVALID_OGL_VALUE;
    vbo = INVALID_OGL_VALUE;
    ibo = INVALID_OGL_VALUE;

    if(load)
        Load();

    if(allocate)
        Allocate();
}

QuadMesh::~QuadMesh()
{
    Deallocate();
    Unload();
}

int QuadMesh::Load(){

    if(!loaded){
        index_count = 6;
        vertex_count = 4;
        index_data = new unsigned int[index_count];
        vertex_data = new vmath::vec2[vertex_count];

        if(vertex_data != 0 && index_data != 0){
            vertex_data[0] = vmath::vec2(0.0f, 0.0f);
            vertex_data[1] = vmath::vec2(1.0f, 0.0f);
            vertex_data[2] = vmath::vec2(1.0f, 1.0f);
            vertex_data[3] = vmath::vec2(0.0f, 1.0f);

            index_data[0] = 0;
            index_data[1] = 1;
            index_data[2] = 2;
            index_data[3] = 0;
            index_data[4] = 2;
            index_data[5] = 3;

            loaded = true;
            return 0;
        }
    }
    return 1;
}

int QuadMesh::Unload(){
    if(loaded){
        delete [] vertex_data;
        delete [] index_data;
        loaded = false;
        return 0;
    }
    return 1;
}

int QuadMesh::Allocate(){
    if(!allocated && loaded){

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(vmath::vec2), vertex_data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        allocated = true;
        return 0;
    }
    return 1;
}

int QuadMesh::Deallocate(){
    if(allocated){

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);

        vao = INVALID_OGL_VALUE;
        vbo = INVALID_OGL_VALUE;
        ibo = INVALID_OGL_VALUE;

        allocated = false;
        return 0;
    }
    return 1;
}

void QuadMesh::Draw(){
    if(allocated){
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}
