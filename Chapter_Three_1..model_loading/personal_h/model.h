#pragma once
#ifndef MODEL_H

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <glad/glad.h> 


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);


class Model
{
public:
    /*  模型数据  */
    vector<Texture> textures_loaded;	// 存储到目前为止加载的所有纹理，并进行优化以确保纹理不会多次加载。
    vector<Mesh>    meshes;          //Model类包含了一个Mesh对象的vector
    string directory;                //储存文件路径的目录，在之后加载纹理的时候还会用到它
    bool gammaCorrection;

    /*  函数   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(const Shader & shader)//遍历了所有网格，并调用它们各自的Draw函数
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
private:

    void loadModel(string path)//加载模型至Assimp的一个叫做scene的数据结构中
    {
		Assimp::Importer importer;
		/*aiProcess_Triangulate:告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形。
         aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标*/
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);


    }
    void processNode(aiNode* node, const aiScene* scene)//一个递归函数,遍历所有的节点和子节点，找到存放的数据，递归函数在做一些处理之后，使用不同的参数递归调用这个函数自身，直到某个条件被满足停止递归
    {
        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //如果有子节点，递归遍历
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)//将Assimp的mesh转换到自己定义的mesh数据结构中
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// 处理顶点位置、法线和纹理坐标
			vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
            {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
			vertices.push_back(vertex);
		}

        /////////////////////////////顶点索引////////////////////
        /*每个网格都有一个面(Face)数组，每个面代表了一个图元,我们遍历所有的面，并储存了面的索引到indices这个vector中就可以了*/
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        //////////////////////////材质纹理/////////////////////
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);

    }


    /*遍历了给定纹理类型的所有纹理位置，获取了纹理的文件位置，并加载并和生成了纹理，将信息储存在了一个Vertex结构体中*/
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);//str获取的是相对路径

            // 每当我们想加载一个纹理的时候，首先去检查它有没有被加载过。如果有的话，我们会直接使用那个纹理，并跳过整个加载流程
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // 已加载具有相同文件路径的纹理，请继续下一个纹理。（优化）
                    break;
                }
            }
            if (!skip)
            {   // 如果纹理尚未加载，请加载它
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // 将其存储为为整个模型加载的纹理，以确保我们不会加载重复纹理。
            }
        }
        return textures;
    }

};


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

#endif // !MODEL_H