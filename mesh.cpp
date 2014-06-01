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

#include <GL/glew.h>
#include <GL/GL.h>
#include "mesh.h"


Mesh::Mesh(char* fileName, bool load) : m_fileName(fileName) {
    if(load)
        Load();
}

Mesh::~Mesh() {
    delete m_fileName;
}

int Mesh::Load(){

    bool result = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(m_fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (pScene) {
        m_entries.resize(pScene->mNumMeshes);

        const aiVector3D zeroValue3f(0.0f);

        for(unsigned int ind = 0; ind < m_entries.size(); ++ind){
            const aiMesh* paiMesh = pScene->mMeshes[ind];

            m_entries[ind].materialIndex = paiMesh->mMaterialIndex;

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            for(unsigned int i = 0; i < paiMesh->mNumVertices; ++i) {
                const aiVector3D* pPosition = &(paiMesh->mVertices[i]);
                const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
                const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zeroValue3f;
                const aiVector3D* pBiNormal = paiMesh->HasTangentsAndBitangents() ? &(paiMesh->mBitangents[i]) : &zeroValue3f;
                const aiVector3D* pTangent =  paiMesh->HasTangentsAndBitangents() ? &(paiMesh->mTangents[i]) : &zeroValue3f;

                Vertex v(vmath::vec3(pPosition->x, pPosition->y, pPosition->z),
                         vmath::vec2(pTexCoord->x, pTexCoord->y),
                         vmath::vec3(pNormal->x, pNormal->y, pNormal->z),
                         vmath::vec3(pBiNormal->x, pBiNormal->y, pBiNormal->z),
                         vmath::vec3(pTangent->x, pTangent->y, pTangent->z)
                );

                vertices.push_back(v);
            }

            for(unsigned int i = 0; i < paiMesh->mNumFaces; ++i) {
                const aiFace& Face = paiMesh->mFaces[i];

                indices.push_back(Face.mIndices[0]);
                indices.push_back(Face.mIndices[1]);
                indices.push_back(Face.mIndices[2]);
            }

            m_entries[ind].Init(vertices, indices);
        }
    }
    else {
        qDebug() << "Importer Error: " << Importer.GetErrorString();
    }

    return result;
}

int Mesh::Unload(){
    return 0;
}

int Mesh::Allocate(){
    return 0;
}

int Mesh::Deallocate(){
    return 0;
}

void Mesh::Draw(){

    for(std::vector<MeshEntry>::iterator it = m_entries.begin(); it != m_entries.end(); it++){
        it->Draw();
    }
}

MeshEntry::MeshEntry()
{
    vao = INVALID_OGL_VALUE;
    vbo = INVALID_OGL_VALUE;
    ibo = INVALID_OGL_VALUE;
    numIndices  = 0;
    materialIndex = INVALID_MATERIAL;
}

MeshEntry::~MeshEntry()
{
    if (vbo != INVALID_OGL_VALUE){
        glDeleteBuffers(1, &vbo);
    }

    if (ibo != INVALID_OGL_VALUE){
        glDeleteBuffers(1, &ibo);
    }

    if(vao != INVALID_OGL_VALUE){
        glDeleteVertexArrays(1, &vao);
    }
}

void MeshEntry::Bind(){
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}

void MeshEntry::Unbind(){

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindVertexArray(0);
}

void MeshEntry::Draw(){
    Bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    Unbind();
}

void MeshEntry::Init(const std::vector<Vertex>& vertices,
                           const std::vector<unsigned int>& indices)
{
    numIndices = indices.size();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}


