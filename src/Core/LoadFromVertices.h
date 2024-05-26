#pragma once

#include <utility>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>

struct BoneInfo {
    glm::mat4 boneOffset;
    glm::mat4 finalTransformation;
};


inline std::pair<int, float*> loadVerticesFromFileOld(std::string fileName, int& numIndices, unsigned int*& indices, std::vector<BoneInfo>& boneInfos, std::map<std::string, unsigned int>& boneMapping, int& numBones)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Error loading model: " << importer.GetErrorString() << std::endl;
        return std::make_pair(0, nullptr);
    }

    int totalNumVertices = 0;
    int totalNumIndices = 0;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        totalNumVertices += mesh->mNumVertices;
        totalNumIndices += mesh->mNumFaces * 3;
    }

    float* vertices = new float[totalNumVertices * 8];
    indices = new unsigned int[totalNumIndices];

    int vertexIndex = 0;
    int indexOffset = 0;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        int numVertices = mesh->mNumVertices;

        for (int j = 0; j < numVertices; j++)
        {
            vertices[vertexIndex++] = mesh->mVertices[j].x;
            vertices[vertexIndex++] = mesh->mVertices[j].y;
            vertices[vertexIndex++] = mesh->mVertices[j].z;

            if (mesh->HasNormals())
            {
                vertices[vertexIndex++] = mesh->mNormals[j].x;
                vertices[vertexIndex++] = mesh->mNormals[j].y;
                vertices[vertexIndex++] = mesh->mNormals[j].z;
            }
            else
            {
                vertices[vertexIndex++] = 0.0f;
                vertices[vertexIndex++] = 0.0f;
                vertices[vertexIndex++] = 0.0f;
            }

            if (mesh->mTextureCoords[0])
            {
                vertices[vertexIndex++] = mesh->mTextureCoords[0][j].x / 3;
                vertices[vertexIndex++] = mesh->mTextureCoords[0][j].y / 3;
            }
            else
            {
                vertices[vertexIndex++] = 0.0f;
                vertices[vertexIndex++] = 0.0f;
            }
        }

        for (int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace face = mesh->mFaces[j];
            for (int k = 0; k < face.mNumIndices; k++)
                indices[indexOffset + j * 3 + k] = face.mIndices[k] + indexOffset;
        }

        indexOffset += mesh->mNumFaces * 3;
    }

    numIndices = totalNumIndices;


    // Load bone information
    //for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    //{
    //    aiMesh* mesh = scene->mMeshes[i];
    //    for (unsigned int j = 0; j < mesh->mNumBones; j++)
    //    {
    //        aiBone* bone = mesh->mBones[j];
    //        std::string boneName(bone->mName.data);

    //        if (boneMapping.find(boneName) == boneMapping.end())
    //        {
    //            BoneInfo boneInfo;
    //            boneInfo.boneOffset = glm::transpose(glm::make_mat4(&bone->mOffsetMatrix.a1));
    //            boneInfos.push_back(boneInfo);
    //            boneMapping[boneName] = numBones;
    //            numBones++;
    //        }
    //    }
    //}

    return std::make_pair(totalNumVertices, vertices);
}