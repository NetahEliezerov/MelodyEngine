#pragma once

#include <utility>
#include <string>
#include <glm/glm.hpp>
#include "../Utils/OBJ_Loader.h"

inline std::pair<int, float*> loadVerticesFromFile(std::string fileName, int& numIndices, unsigned int*& indices)
{
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(fileName);
    objl::Mesh curMesh = Loader.LoadedMeshes[0];

    int numVertices = curMesh.Vertices.size();
    float* vertices = new float[numVertices * 8];

    int index = 0;
    for (int j = 0; j < numVertices; j++) {
        vertices[index++] = curMesh.Vertices[j].Position.X;
        vertices[index++] = curMesh.Vertices[j].Position.Y;
        vertices[index++] = curMesh.Vertices[j].Position.Z;
        vertices[index++] = curMesh.Vertices[j].Normal.X;
        vertices[index++] = curMesh.Vertices[j].Normal.Y;
        vertices[index++] = curMesh.Vertices[j].Normal.Z;
        vertices[index++] = curMesh.Vertices[j].TextureCoordinate.X;
        vertices[index++] = curMesh.Vertices[j].TextureCoordinate.Y;
    }

    numIndices = curMesh.Indices.size();
    indices = new unsigned int[numIndices];

    for (int i = 0; i < numIndices; i++) {
        indices[i] = curMesh.Indices[i];
    }

    return std::make_pair(numVertices, vertices);
}