#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"


#include <string>
#include<vector>

using namespace std;



//��������
struct Vertex {
    glm::vec3 Position;//λ��
    glm::vec3 Normal;//����
    glm::vec2 TexCoords;//��������
};

//����
struct Texture {
    unsigned int id;//����ID
    string type;//��������
    string path;//����·��
};



class Mesh {
public:
    /*  ��������  */
    vector<Vertex> vertices;//һϵ�ж���ṹ��
    vector<unsigned int> indices;//��������
    vector<Texture> textures;//��ͼ�ṹ��


    /*  ����  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }
    void Draw(const Shader & shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
            // ��ȡ������ţ�diffuse_textureN �е� N��
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // ��������
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }



private:
    /*  ��Ⱦ����  */
    unsigned int VAO, VBO, EBO;

    //������ȷ�Ļ��壬��ͨ����������ָ�붨�嶥����ɫ���Ĳ���
    void setupMesh()
    {
        //����һ��VAO��VBO��EBO����
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);


        //��objVAO
        glBindVertexArray(VAO);

        //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);




        //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
        //��һ��0������ɫ�����layout (location = 0)������λ��ֵ
        //�ṹ�������һ���ܺõ���;������Ԥ����ָ��offsetof(s, m)�����ĵ�һ��������һ���ṹ�壬�ڶ�������������ṹ���б��������֡������᷵���Ǹ�������ṹ��ͷ�����ֽ�ƫ����(Byte Offset)��
        //λ������
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        //��������
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        //������������
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
    }
};



#endif // !MESH_H
