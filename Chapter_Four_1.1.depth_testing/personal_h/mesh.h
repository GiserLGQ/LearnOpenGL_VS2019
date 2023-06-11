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



//顶点数据
struct Vertex {
    glm::vec3 Position;//位置
    glm::vec3 Normal;//法线
    glm::vec2 TexCoords;//纹理坐标
};

//纹理
struct Texture {
    unsigned int id;//纹理ID
    string type;//纹理类型
    string path;//材质路径
};



class Mesh {
public:
    /*  网格数据  */
    vector<Vertex> vertices;//一系列顶点结构体
    vector<unsigned int> indices;//顶点索引
    vector<Texture> textures;//贴图结构体


    /*  函数  */
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
            glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
            // 获取纹理序号（diffuse_textureN 中的 N）
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

        // 绘制网格
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }



private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;

    //配置正确的缓冲，并通过顶点属性指针定义顶点着色器的布局
    void setupMesh()
    {
        //生成一个VAO、VBO、EBO对象
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);


        //绑定objVAO
        glBindVertexArray(VAO);

        //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //把之前定义的顶点数据复制到缓冲的内存中
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);




        //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
        //第一个0就是着色器里的layout (location = 0)个属性位置值
        //结构体的另外一个很好的用途是它的预处理指令offsetof(s, m)，它的第一个参数是一个结构体，第二个参数是这个结构体中变量的名字。这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)。
        //位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        //法线属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        //纹理坐标属性
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
    }
};



#endif // !MESH_H
