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
    /*  ģ������  */
    vector<Texture> textures_loaded;	// �洢��ĿǰΪֹ���ص����������������Ż���ȷ���������μ��ء�
    vector<Mesh>    meshes;          //Model�������һ��Mesh�����vector
    string directory;                //�����ļ�·����Ŀ¼����֮����������ʱ�򻹻��õ���
    bool gammaCorrection;

    /*  ����   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(const Shader & shader)//�������������񣬲��������Ǹ��Ե�Draw����
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
private:

    void loadModel(string path)//����ģ����Assimp��һ������scene�����ݽṹ��
    {
		Assimp::Importer importer;
		/*aiProcess_Triangulate:����Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�
         aiProcess_FlipUVs���ڴ����ʱ��תy�����������*/
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);


    }
    void processNode(aiNode* node, const aiScene* scene)//һ���ݹ麯��,�������еĽڵ���ӽڵ㣬�ҵ���ŵ����ݣ��ݹ麯������һЩ����֮��ʹ�ò�ͬ�Ĳ����ݹ���������������ֱ��ĳ������������ֹͣ�ݹ�
    {
        // ����ڵ����е���������еĻ���
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //������ӽڵ㣬�ݹ����
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)//��Assimp��meshת�����Լ������mesh���ݽṹ��
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// ������λ�á����ߺ���������
			vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
            {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
			vertices.push_back(vertex);
		}

        /////////////////////////////��������////////////////////
        /*ÿ��������һ����(Face)���飬ÿ���������һ��ͼԪ,���Ǳ������е��棬�����������������indices���vector�оͿ�����*/
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        //////////////////////////��������/////////////////////
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


    /*�����˸����������͵���������λ�ã���ȡ��������ļ�λ�ã������ز�����������������Ϣ��������һ��Vertex�ṹ����*/
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);//str��ȡ�������·��

            // ÿ�����������һ�������ʱ������ȥ�������û�б����ع�������еĻ������ǻ�ֱ��ʹ���Ǹ�����������������������
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // �Ѽ��ؾ�����ͬ�ļ�·���������������һ���������Ż���
                    break;
                }
            }
            if (!skip)
            {   // ���������δ���أ��������
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // ����洢ΪΪ����ģ�ͼ��ص�������ȷ�����ǲ�������ظ�����
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